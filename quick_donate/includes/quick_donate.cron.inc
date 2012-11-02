<?php

/**
 * @file cron page callbacks and associated helper functions.
 */

/**
 * Run routine maintenance tasks required by quick donate.
 */
function quick_donate_cron_tasks() {
  watchdog('quick_donate_cron', t('Quick Donate cron invoked'));
  // process card expiration warnings.
  if (variable_get('quick_donate_card_expiration_confirmations', '0')) {
    quick_donate_process_expiration_emails();
  }
}


function quick_donate_process_expiration_emails() {
  $intervals = variable_get('quick_donate_card_expiration_intervals', '');
  if ($intervals) {
    $intervals = explode(',', $intervals);
    foreach ($intervals as $interval) {
      $expiration_timestamp = time() + (86400 * (int) trim($interval));
      $expiring_payment_methods = _quick_donate_expiring_payment_methods($interval, $expiration_timestamp);
      foreach ($expiring_payment_methods as $method) {
        quick_donate_send_pm_expiration_mail($method['uid'], $method['pid']);
        quick_donate_update_expiration_interval($method['pid'], $interval);
      }
    }
  }
}

function _quick_donate_expiring_payment_methods($interval, $expiration_timestamp) {
  $methods = array();
  $sql = sprintf('
    SELECT
      uid,
      pid
    FROM {quick_donate_pm_expiration}
    WHERE
      expiration_timestamp <= %d
    AND
      (
        last_interval_checked > %d
        OR last_interval_checked = 0
      )
  ', $expiration_timestamp, $interval);
  $results = db_query($sql);
  while ($method = db_fetch_array($results)) {
    $methods[] = $method;
  }
  watchdog('quick_donate_cron', count($methods) . " expiring methods found.");
  return $methods;
}