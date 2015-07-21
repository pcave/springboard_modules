<?php

/**
 * Callback handler for 'node/%/performance' menu item.
 *
 * Generates and outputs deliverability reports for a particular message action.
 */
function sba_message_action_performance_page($nid) {
  // CSS and JS assets
  drupal_add_css(drupal_get_path('module', 'sba_message_action') . '/css/message-performance.css');

  // Load up the message action node.
  $node = node_load($nid);

  // API Client instance
  $client = springboard_advocacy_api_call();

  // Try to make the API call and if it fails return a response.
  $report = $client->invokeClientMethod('getTargetDeliverability', $node->advocacy_id);

  // Check the status of the API call return and if it's an error return that to the browser.
  if (isset($report->error)):
    return 'Sorry, no performance metrics are available for this message.';
  endif;

  $deliverablity_targets = sba_message_action_performance_targets_table($report, $node);

  return $deliverablity_targets;
}

/**
 * Helper function to convert target deliverability report data into
 * a data structure that can be rendered in an HTML table.
 */
function sba_message_action_performance_targets_table($report, $node) {

  $targets = !empty($report->data->targets) ? $report->data->targets : array();

  $rows = array();

  $nodeId = $node->advocacy_id;

  // Get an instance of the advocacy API client.
  $client = springboard_advocacy_api_call();

  foreach ($targets as $target) {

    // Get the target deliverability metrics for this target
    $targetDeliverability = $client->invokeClientMethod('getSingleTargetDeliverability', $nodeId, $target->id);

    if (!empty($targetDeliverability->data)) {
      $rows[] = array(
        $target->salutation.' '.$target->first_name.' '.$target->last_name,
        $target->party,
        $target->state,
        $target->district_code,
        $targetDeliverability->data->totalMessages,
        $targetDeliverability->data->delivered,
        $targetDeliverability->data->failed,
        $targetDeliverability->data->deliverabilityIndexPercentage
      );
    }
  }

  // Define the table header
  $header = array('Name', 'Party', 'State', 'District', 'Total Messages Sent', 'Delivered Messages', 'Failed Messages', 'Deliverability Index');

  $queueReady = $client->invokeClientMethod('getQueueMessagesCountByStatus', $nodeId, 'ready');
  $queuePaused = $client->invokeClientMethod('getQueueMessagesCountByStatus', $nodeId, 'paused');
  $queueCanceled = $client->invokeClientMethod('getQueueMessagesCountByStatus', $nodeId, 'canceled');
  $queueReadyItems = !empty($queueReady->data->count) ? $queueReady->data->count : 0;
  $queuePausedItems = !empty($queuePaused->data->count) ? $queuePaused->data->count : 0;
  $queueCanceledItems = !empty($queueCanceled->data->count) ? $queueCanceled->data->count : 0;

  $tabs = theme('messsage_deliverability_tabs', array(
    'nodeId' => $node->nid
  ));

  $output = theme('messsage_deliverability_dashboard', array(
    'targets_table_header' => $header,
    'targets'              => $rows,
    'report'               => $report,
    'queueReadyItems'      => $queueReadyItems,
    'queuePausedItems'     => $queuePausedItems,
    'queueCanceledItems'   => $queueCanceledItems,
    'nodeId'               => $node->nid,
    'tabs'                 => $tabs
  ));
  return $output;
}

/**
 * Implements hook_preprocess()
 * @param $variables
 * @throws \Exception
 */
function sba_message_action_preprocess_messsage_deliverability_dashboard(&$vars) {
  // Sanitize the targets data
  foreach ($vars['targets'] as $key => $data) {
    foreach ($data as $itemKey => $value) {
      $vars['targets'][$key][$itemKey] = check_plain($value);
    }
  }

  // Sanitize report data
    $vars['totalMessages'] = isset($vars['report']->data->totalMessages) ? check_plain($vars['report']->data->totalMessages) : 0;
    $vars['delivered'] = isset($vars['report']->data->delivered) ? check_plain($vars['report']->data->delivered) : 0;
    $vars['failed'] = isset($vars['report']->data->failed) ? check_plain($vars['report']->data->failed) : 0;
    $vars['percentage'] = isset($vars['report']->data->deliverabilityIndexPercentage) ? check_plain($vars['report']->data->deliverabilityIndexPercentage) : 0;

  // Render targets table
  $targetsTable = theme('table', array('header' => $vars['targets_table_header'], 'rows' => $vars['targets']));

  $vars['targets_table'] = $targetsTable;
}

