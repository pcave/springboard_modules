<?php

/**
 * Admin settings form.
 */
function quick_donate_admin() {
  $form = array();
  $form['quick_donate_debug_mode'] = array(
    '#type' => 'checkbox',
    '#title' => t('Quick Donate debug mode'),
    '#default_value' => variable_get('quick_donate_debug_mode', 0),
  );
  $form['registration_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Registation settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['registration_settings']['quick_donate_register_sustainers'] = array(
    '#type' => 'checkbox',
    '#title' => t('Automatically sign up sustainers for Quick Donate'),
    '#default_value' => variable_get('quick_donate_register_sustainers', 0),
  );
  $form['registration_settings']['quick_donate_thank_you_page'] = array(
    '#type' => 'textfield',
    '#title' => t('Thank you page'),
    '#description' => t('The Drupal internal path (eg. node/42) to redirect users to when they have signed up for Quick Donate.'),
    '#default_value' => variable_get('quick_donate_thank_you_page', FALSE),
  );
  $form['registration_settings']['quick_donate_registration_message'] = array(
    '#type' => 'textarea',
    '#title' => t('Quick Donate registration message'),
    '#description' => t('This message is displayed on the Quick Donate registration form. It should include an explanation of the service and some indication of how the user benefits from registering.'),
    '#default_value' => variable_get('quick_donate_registration_message', t('The quick donate service allows you to streamline the donation process by replacing tedious forms with a single one-click donation experience. Quick Donate also allows you to bypass the website entirely, permitting you to conveniently donate from your phone via SMS.')),
  );
  

  // user confirmation email settings
  $form['email_settings'] = array(
    '#type'=> 'fieldset',
    '#title' => t('Registration email settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['email_settings']['quick_donate_registration_confirmations'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Quick Donate registration emails on signup'),
    '#description' => t('When enabled any user who signs up for Quick Donate will receive a registration email using the template settings below.'),
    '#default_value' => variable_get('quick_donate_registration_confirmations', '1'),
  );
  $form['email_settings']['quick_donate_registration_from_address'] = array(
    '#type' => 'textfield',
    '#title' => t('From email'),
    '#description' => t(''),
    '#default_value' => variable_get('quick_donate_registration_from_address', ''),
  );
  $form['email_settings']['quick_donate_registration_from_name'] = array(
    '#type' => 'textfield',
    '#title' => t('From name'),
    '#description' => t(''),
    '#default_value' => variable_get('quick_donate_registration_from_name', ''),
  );
  $form['email_settings']['quick_donate_registration_bcc_address'] = array(
    '#type' => 'textfield',
    '#title' => t('BCC'),
    '#description' => t('Comma separated list of email addresses that will recieve a BCC copy of any Quick Donate registration emails. (example: test@example.com,test2@example.com)'),
    '#default_value' => variable_get('quick_donate_registration_bcc_address', ''),
  );
  $form['email_settings']['quick_donate_registration_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#description' => t('The subject line for Quick Donate registration emails.'),
    '#default_value' => variable_get('quick_donate_registration_subject', ''),
  );
  // html message
  // text message
  $form['email_settings']['quick_donate_registration_html_email'] = array(
    '#type' => 'textarea',
    '#title' => t('HTML message'),
    '#description' => t('HTML version of the Quick Donate registration email.'),
    '#default_value' => variable_get('quick_donate_registration_html_email', ''),
  );
  $form['email_settings']['html_tokens'] = array(
    '#type' => 'fieldset',
    '#title' => t('available tokens'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['email_settings']['html_tokens']['quick_donate_html_token_help'] = array(
    '#type' => 'markup',
    '#value' => theme('token_help', 'quick_donate'),
  );
  $form['email_settings']['quick_donate_registration_text_email'] = array(
    '#type' => 'textarea',
    '#title' => t('Text message'),
    '#description' => t('Plaintext version of the Quick Donate registration email.'),
    '#default_value' => variable_get('quick_donate_registration_text_email', ''),
  );
  $form['email_settings']['text_tokens'] = array(
    '#type' => 'fieldset',
    '#title' => t('available tokens'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['email_settings']['text_tokens']['quick_donate_text_token_help'] = array(
    '#type' => 'markup',
    '#value' => theme('token_help', 'quick_donate'),
  );
  $form['auth_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Authentication & Security'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['auth_settings']['quick_donate_2nd_auth_period'] = array(
    '#type' => 'textfield',
    '#title' => t('2nd auth duration'),
    '#size' => 10,
    '#description' => t('Time (in seconds) to keep a user authenticated when they complete 2nd auth to view payment details. Defaults to 5 minutes (9600)'),
    '#default_value' => variable_get('quick_donate_2nd_auth_period', 300),
  );
  $form['auth_settings']['quick_donate_auth_lede'] = array(
    '#type' => 'textarea',
    '#title' => t('Payment Method 2nd auth message'),
    '#description' => t('A message to display to the user explaining the need for the authentication page guarding the payment method details pages.'),
    '#default_value' => variable_get('quick_donate_auth_lede', t('For your protection you must reauthenticate to view payment method details.')),
  );
  /*
    $form['email_settings']['quick_donate_sustainer_upgrade_confirmations'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable confirmation emails when a sustainer is upgraded.'),
    '#description' => t('If enabled a confirmation email will be sent when a sustainer upgrades to a higher donation amount.'),
    '#default_value' => variable_get('quick_donate_sustainer_upgrade_confirmations', '0'),
  );
    $form['email_settings']['quick_donate_sustainer_conversion_confirmations'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable confirmation emails on sustainer conversions'),
    '#description' => t('If enabled a confirmation email will be sent when a donor converts to a sustainer.'),
    '#default_value' => variable_get('quick_donate_sustainer_conversion_confirmations', '0'),
  );
  
  // Service response settings, these responses are sent to the service caller and may differ from
  // confirmations sent to the user via other means.
    $form['response_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Response settings'),
    '#description' => t('Configure the responses sent to 3rd party services.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  
  // donation service response
  $form['response_settings']['quick_donate_donation_response'] = array(
    '#type' => 'textarea',
    '#title' => t('Donation response message (text only, no markup)'),
    '#description' => t(''),
    '#default_value' => variable_get('quick_donate_donation_response', t('')),
  );
  $form['response_settings']['donation_tokens'] = array(
    '#type' => 'fieldset',
    '#title' => t('Available sustainer message tokens'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['response_settings']['donation_tokens']['help'] = array(
    '#value' => theme('token_help', 'quick_donate'),
  );
  
  // Sustainer upgrade response
  $form['response_settings']['quick_donate_sustainer_upgrade_response'] = array(
    '#type' => 'textarea',
    '#title' => t('Sustainer upgrade response message (text only, no markup)'),
    '#description' => t(''),
    '#default_value' => variable_get('quick_donate_sustainer_upgrade_response', t('')),
  );
  $form['response_settings']['sustainer_upgrade_tokens'] = array(
    '#type' => 'fieldset',
    '#title' => t('Available sustainer message tokens'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['response_settings']['sustainer_upgrade_tokens']['help'] = array(
    '#value' => theme('token_help', 'quick_donate'),
  );
  
  // Sustainer conversion response
  $form['response_settings']['quick_donate_sustainer_conversion_response'] = array(
    '#type' => 'textarea',
    '#title' => t('Sustainer conversion response message (text only, no markup)'),
    '#description' => t(''),
    '#default_value' => variable_get('quick_donate_sustainer_conversion_response', t('')),
  );
    $form['response_settings']['sustainer_conversion_tokens'] = array(
    '#type' => 'fieldset',
    '#title' => t('Available sustainer message tokens'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['response_settings']['sustainer_conversion_tokens']['help'] = array(
    '#value' => theme('token_help', 'quick_donate'),
  );
  */
  return system_settings_form($form);
}

