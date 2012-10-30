<?php

/**
 * @file Rules integration
 */


/**
 * Implements hook_rules_event_info().
 */
function quick_donate_rules_event_info() {
  drupal_set_message('rules event info invoked');
 return array(
   'quick_donate_payment_method_expiration' => array(
     'label' => t('A payment method is about to expire.'),
     'module' => 'quick_donate',
     'arguments' => array(
       'associated_user' => array(
         'type' => 'user',
         'label' => t('User associated with the expiring payment method.')
       ),
       'pid' => array(
         'type' => 'value',
         'label' => t('The id of the expiring payment method.'),
       ),
     ),
   ),
 );
}


function quick_donate_rules_action_info() {
  return array(
    'quick_donate_expiration_warning' => array(
      'label' => t('Send payment method expiration warning email'),
      'arguments' => array(
        'account' => array(
          'type' => 'user',
          'label' => t('User associated with the expiring payment method.'),
        ),
        'pid' => array(
          'type' => 'value',
          'label' => t('ID of the expiring payment method'),
        ),
      ),
      'module' => 'quick_donate',
    ),
  );
}


// event handler: send expiration warning email.
function quick_donate_payment_method_expiration() {

}

function quick_donate_expiration_warning($account, $pid) {

}