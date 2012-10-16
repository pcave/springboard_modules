<?php

/**
 * @file
 */


function quick_donate_donation() {
  $params = _quick_donate_query_args();
  if (!$params) {
    // TODO: return error message and bail
    drupal_set_message('one or more required parameters missing.');
  }

  $account = user_load(array('uid' => $params['uid']));

  drupal_set_message('account:' . print_r($account, TRUE));
  drupal_set_message('params:' . print_r($params, TRUE));
  
  $node = node_load($params['fid']);
  
  if (!_fundraiser_is_donation_type($node)) {
    // TODO: throw error, referenced node is not a donation form.
  }
  
  // TODO: tweak fundraiser to check for reference flag and run reference transaction instead of cc processing.
  
  // TODO: response handling - perform token conversion on service response message
  /* TODO: email - if confirmation email is disabled
   *  check for Email Confirmations. If present, jam email confirmations outbound mail. If not present, jam Ubercart CA confirmation.
   *  Otherwise, email should happen automagically during donation processing.
   */
  
  // TODO: write function to return XML response.
  
  // if response type is specified in the query string, return a response in the requested format type.
  // valid response types: xml, json
  $response_types = array('xml', 'json');
  if (isset($params['resp']) && in_array($resp, $response_types)) {
    // TODO: return xml
    // TODO: return json
  }
  // TODO: return response message as normal Drupal page.
  return 'derp';
}


function quick_donate_sustainer_ugrade() {
  $params = _quick_donate_query_args();
}


function quick_donate_sustainer_conversion($uid, $amount) {

}


/**
 * Produce a list of market source custom field keys.
 */
function quick_donate_market_source_fields() {
  if (module_exists('market_source')) {
    $fields = array('ms', 'cid');
    $custom_fields = variable_get('market_source_custom_fields', array());
    
    if (count($custom_fields)) {
      
      foreach ($custom_fields as $custom_field) {
        $fields[] = $custom_field['key'];
      }
      return $fields;
    }
  }
  return FALSE;
}

function _quick_donate_query_args() {
  $params = array();
  $required_fields = array('uid', 'amt', 'fid');
  $custom_fields = quick_donate_market_source_fields();
  $fields = ($custom_fields) ? array_merge($required_fields, $custom_fields) : $required_fields;
  
  $keys = array_keys($_GET);
  foreach ($fields as $field) {
    if (!isset($_GET[$field]) && in_array($field, $required_fields)) {
      // TODO: replace with watchdog().
      drupal_set_message('missing required value:' . $field);
      return FALSE;
    }
    elseif (isset($_GET[$field])) {
      $params[$field] = $_GET[$field];
    }
  }
  return $params;
}