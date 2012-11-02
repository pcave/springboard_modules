<?php

/**
 * Admin settings form.
 */
function quick_donate_admin($edit = array()) {
  $form = array();
  $form['quick_donate_debug_mode'] = array(
    '#type' => 'checkbox',
    '#title' => t('Quick Donate debug mode'),
    '#default_value' => variable_get('quick_donate_debug_mode', 0),
  );
  $form['registration_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Registration form settings'),
    '#description' => t('Configure the registration form message, button text, and redirect location.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
 /* $form['registration_settings']['quick_donate_register_sustainers'] = array(
    '#type' => 'checkbox',
    '#title' => t('Automatically sign up sustainers for Quick Donate'),
    '#default_value' => variable_get('quick_donate_register_sustainers', 0),
  );*/
  $form['registration_settings']['quick_donate_thank_you_page'] = array(
    '#type' => 'textfield',
    '#title' => t('Thank you page'),
    '#description' => t('The Drupal internal path (eg. node/42) to redirect users to when they have signed up for Quick Donate.'),
    '#default_value' => variable_get('quick_donate_thank_you_page', FALSE),
  );
  $form['registration_settings']['quick_donate_registration_button'] = array(
    '#type' => 'textfield',
    '#title' => t('Registration button text'),
    '#description' => t('This setting allows you to customize the text displayed on the quick donate registration button.'),
    '#default_value' => variable_get('quick_donate_registration_button', 'Register for quick donate'),
  );
  $form['registration_settings']['quick_donate_registration_message'] = array(
    '#type' => 'textarea',
    '#title' => t('Quick Donate registration message'),
    '#description' => t('This message is displayed on the Quick Donate registration form. It should include an explanation of the service and some indication of how the user benefits from registering.'),
    '#default_value' => variable_get('quick_donate_registration_message', t('The quick donate service allows you to streamline the donation process by replacing tedious forms with a streamlined one-click donation experience.')),
  );
    
  // authentication settings.
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

  return system_settings_form($form);
}


function quick_donate_admin_email($edit) {
  $path = drupal_get_path('module', 'quick_donate') . '/styles/quick_donate.admin.css';
  drupal_add_css($path);
  $form['registration'] = array(
    '#type' => 'fieldset',
    '#title' => t('Quick Donate Registration Email'),
    '#description' => t('Configure mail template & settings when a user enrolls in Quick Donate'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['registration']['quick_donate_registration_confirmations'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Quick Donate registration emails on signup'),
    '#description' => t('When enabled any user who signs up for Quick Donate will receive a registration email using the template settings below.'),
    '#default_value' => variable_get('quick_donate_registration_confirmations', '1'),
  );
  $form['registration']['quick_donate_registration_from_address'] = array(
    '#type' => 'textfield',
    '#title' => t('From email'),
    '#description' => t(''),
    '#default_value' => variable_get('quick_donate_registration_from_address', ''),
  );
  $form['registration']['quick_donate_registration_from_name'] = array(
    '#type' => 'textfield',
    '#title' => t('From name'),
    '#description' => t(''),
    '#default_value' => variable_get('quick_donate_registration_from_name', ''),
  );
  $form['registration']['quick_donate_registration_bcc_address'] = array(
    '#type' => 'textfield',
    '#title' => t('BCC'),
    '#description' => t('Comma separated list of email addresses that will recieve a BCC copy of any Quick Donate registration emails. (example: test@example.com,test2@example.com)'),
    '#default_value' => variable_get('quick_donate_registration_bcc_address', ''),
  );
  $form['registration']['quick_donate_registration_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#description' => t('The subject line for Quick Donate registration emails.'),
    '#default_value' => variable_get('quick_donate_registration_subject', ''),
  );
  // html message
  // text message
  $form['registration']['quick_donate_registration_html_email'] = array(
    '#type' => 'textarea',
    '#title' => t('HTML message'),
    '#description' => t('HTML version of the Quick Donate registration email.'),
    '#default_value' => variable_get('quick_donate_registration_html_email', ''),
  );
  $form['registration']['html_tokens'] = array(
    '#type' => 'fieldset',
    '#title' => t('available tokens'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['registration']['html_tokens']['quick_donate_registration_html_token_help'] = array(
    '#type' => 'markup',
    '#value' => theme('token_help', 'quick_donate'),
  );
  $form['registration']['quick_donate_registration_text_email'] = array(
    '#type' => 'textarea',
    '#title' => t('Text message'),
    '#description' => t('Plaintext version of the Quick Donate registration email.'),
    '#default_value' => variable_get('quick_donate_registration_text_email', ''),
  );
  $form['registration']['text_tokens'] = array(
    '#type' => 'fieldset',
    '#title' => t('available tokens'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['registration']['text_tokens']['quick_donate_registration_text_token_help'] = array(
    '#type' => 'markup',
    '#value' => theme('token_help', 'quick_donate'),
  );
  
  // card expiring message
  $form['card_expiration'] = array(
    '#type' => 'fieldset',
    '#title' => t('Quick Donate Payment Method Expiration Email'),
    '#description' => t('Configure mail template & settings when a credit card is about to expire'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['card_expiration']['quick_donate_card_expiration_confirmations'] = array(
    '#type' => 'checkbox',
    '#title' => t("Send a warning email when a stored payment method is about to expire."),
    '#description' => t('When enabled any user who signs up for Quick Donate will receive a card expiration email using the template settings below.'),
    '#default_value' => variable_get('quick_donate_card_expiration_confirmations', '1'),
  );
  $form['card_expiration']['quick_donate_card_expiration_intervals'] = array(
    '#type' => 'textfield',
    '#title' => t('Card expiration warning intervals'),
    '#description' => t('Send an expiration warning email this many days before a card expires. If expiration warning emails are enabled, users who have not opted out will receive a warning email using the template settings below. <strong>Note: If you wish to warn on multiple intervals (ex. 30 days, 60 days, 90 days) use comma-separated values. Example: 30,60,90</strong>'),
    '#default_value' => variable_get('quick_donate_card_expiration_intervals', '30'),
  );
  $form['card_expiration']['quick_donate_card_expiration_from_address'] = array(
    '#type' => 'textfield',
    '#title' => t('From email'),
    '#description' => t(''),
    '#default_value' => variable_get('quick_donate_card_expiration_from_address', ''),
  );
  $form['card_expiration']['quick_donate_card_expiration_from_name'] = array(
    '#type' => 'textfield',
    '#title' => t('From name'),
    '#description' => t(''),
    '#default_value' => variable_get('quick_donate_card_expiration_from_name', ''),
  );
  $form['card_expiration']['quick_donate_card_expiration_bcc_address'] = array(
    '#type' => 'textfield',
    '#title' => t('BCC'),
    '#description' => t('Comma separated list of email addresses that will recieve a BCC copy of any Quick Donate card_expiration emails. (example: test@example.com,test2@example.com)'),
    '#default_value' => variable_get('quick_donate_card_expiration_bcc_address', ''),
  );
  $form['card_expiration']['quick_donate_card_expiration_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#description' => t('The subject line for Quick Donate card_expiration emails.'),
    '#default_value' => variable_get('quick_donate_card_expiration_subject', ''),
  );
  // html body
  $form['card_expiration']['quick_donate_card_expiration_html_email'] = array(
    '#type' => 'textarea',
    '#title' => t('HTML message'),
    '#description' => t('HTML version of the Quick Donate card expiration email.'),
    '#default_value' => variable_get('quick_donate_card_expiration_html_email', ''),
  );
  $form['card_expiration']['html_tokens'] = array(
    '#type' => 'fieldset',
    '#title' => t('available tokens'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['card_expiration']['html_tokens']['quick_donate_registration_html_token_help'] = array(
    '#type' => 'markup',
    '#value' => theme('token_help', 'quick_donate_expire'),
  );
  // text body
  $form['card_expiration']['quick_donate_card_expiration_text_email'] = array(
    '#type' => 'textarea',
    '#title' => t('Text message'),
    '#description' => t('Plaintext version of the Quick Donate card expiration email.'),
    '#default_value' => variable_get('quick_donate_card_expiration_text_email', ''),
  );
  $form['card_expiration']['text_tokens'] = array(
    '#type' => 'fieldset',
    '#title' => t('available tokens'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['card_expiration']['text_tokens']['quick_donate_card_expiration_text_token_help'] = array(
    '#type' => 'markup',
    '#value' => theme('token_help', 'quick_donate_expire'),
  );
  return system_settings_form($form);
}


function _quick_donate_expiration_send_form($edit) {

  // Add a blank row at the end.
  $interval_field_values[] = array();
  $form['interval_wrapper'] = array(
    '#prefix' => '<div id="quick-donate-interval-wrapper">',
    '#suffix' => '</div>',
    '#theme' => 'quick_donate_interval_table',
  );
  
  foreach ($interval_field_values as $i => $field_value) {
    $form['interval_wrapper'][$i] = array();
    $form['interval_wrapper'][$i]['interval'] = array(
      '#type' => 'textfield',
      '#title' => t('Warning interval'),
      '#description' => t("Warn this many days before expiration."),
      '#default_value' => isset($field_value['interval']) ? $field_value['interval'] : '',
      '#size' => 5,
    );
    // cr
    $form['interval_wrapper'][$i]['delete_' . $i] = array(
      '#type' => 'button',
      '#title' => t('remove'),
      "#value" => t('remove'),
      '#submit' => '_quick_donate_remove_interval',
    );
  }

  $form['add_more'] = array(
    '#type' => 'submit',
    '#value' => t('Add interval'),
    '#ahah' => array(
      'path' => 'quick_donate/interval-ahah',
      'wrapper' => 'quick-donate-interval-wrapper',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );
  return $form;
}

