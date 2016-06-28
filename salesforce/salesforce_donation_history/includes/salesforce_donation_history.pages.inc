<?php

/**
 * @file
 *  Page callbacks for donation history.
 */

/**
 * Page callback.
 */
function salesforce_donation_history_page($user) {
  $output = array();
  $results = array();
  $params = drupal_get_query_parameters();
  $sfid = salesforce_donation_history_user_account_id($user->uid);
  if ($sfid) {
    $results = salesforce_donation_history_sort_by_year(salesforce_donation_history_get_opportunities($sfid));
  }
  // Check to see if there are pending donations in the salesforce sync queue.
  if ($pending_count = salesforce_donation_history_count_pending_donations($user->uid)) {
    drupal_set_message(t('You have :count donation(s) in-process. Donations may take up to 30 minutes to appear here.', array(':count' => $pending_count)));
  }
  if (count($results) > 0) {
    // @TODO what happens when we default to displaying the current year but
    // there are only donations from previous years?
    $selected_year = isset($params['year']) ? $params['year'] : date('Y');
    $available_years = array_keys($results);
    $output['select-year'] = drupal_get_form('salesforce_donation_history_select_year', $available_years, $selected_year);
    $output['annual-tax-receipt'] = array(
      '#type' => 'markup',
      '#prefix' => '<div>',
      '#suffix' => '</div>',
      '#markup' => l(t('Print Annual Tax Reciept'), 'user/' . $user->uid . '/donation-history/summary/' . $selected_year),
    );
    $output['table'] = salesforce_donation_history_create_table($results[$selected_year]);
  }
  else {
    $output['no-donations'] = array(
      '#type' => 'markup',
      '#markup' => t('No donations found.'),
    );
  }
  return $output;
}

/**
 * Page callback for annual tax summary.
 */
function salesforce_donation_history_annual_summary_page($user, $year) {
  $output = array();
  $intro = variable_get('salesforce_donation_history_receipts_intro', NULL);
  $output['intro'] = array(
    '#type' => 'markup',
    '#markup' => check_markup($intro['value'], $intro['format']),
  );
  $sfid = salesforce_donation_history_user_account_id($user->uid);
  if ($sfid) {
    $results = salesforce_donation_history_sort_by_year(salesforce_donation_history_get_opportunities($sfid));
  }
  if (count($results) > 0) {
    $output['table'] = salesforce_donation_history_create_table($results[$year], 'show_on_summary', TRUE, FALSE);
  }
  else {
    $output['no-donations'] = array(
      '#type' => 'markup',
      '#markup' => t('No donations found.'),
    );
  }
  $outro = variable_get('salesforce_donation_history_receipts_outro', NULL);
  $output['footer'] = array(
    '#type' => 'markup',
    '#markup' => check_markup($outro['value'], $outro['format']),
  );
  return $output;
}