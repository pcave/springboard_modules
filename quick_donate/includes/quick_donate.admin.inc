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
    '#default_value' => variable_get('quick_donate_registration_message', t('The quick donate service allows you to streamline the donation process by replacing tedious forms with a single one-click donation experience. Quick Donate also allows you to bypass the website entirely, permitting you to conveniently donate from your phone via SMS.')),
  );
  

  // email settings
  $form['email_settings'] = array(
    '#type'=> 'fieldset',
    '#title' => t('Email settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['email_settings']['registration'] = array(
    '#type' => 'fieldset',
    '#title' => t('Quick Donate Registration Email'),
    '#description' => t('Configure mail template & settings when a user enrolls in Quick Donate'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['email_settings']['registration']['quick_donate_registration_confirmations'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Quick Donate registration emails on signup'),
    '#description' => t('When enabled any user who signs up for Quick Donate will receive a registration email using the template settings below.'),
    '#default_value' => variable_get('quick_donate_registration_confirmations', '1'),
  );
  $form['email_settings']['registration']['quick_donate_registration_from_address'] = array(
    '#type' => 'textfield',
    '#title' => t('From email'),
    '#description' => t(''),
    '#default_value' => variable_get('quick_donate_registration_from_address', ''),
  );
  $form['email_settings']['registration']['quick_donate_registration_from_name'] = array(
    '#type' => 'textfield',
    '#title' => t('From name'),
    '#description' => t(''),
    '#default_value' => variable_get('quick_donate_registration_from_name', ''),
  );
  $form['email_settings']['registration']['quick_donate_registration_bcc_address'] = array(
    '#type' => 'textfield',
    '#title' => t('BCC'),
    '#description' => t('Comma separated list of email addresses that will recieve a BCC copy of any Quick Donate registration emails. (example: test@example.com,test2@example.com)'),
    '#default_value' => variable_get('quick_donate_registration_bcc_address', ''),
  );
  $form['email_settings']['registration']['quick_donate_registration_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#description' => t('The subject line for Quick Donate registration emails.'),
    '#default_value' => variable_get('quick_donate_registration_subject', ''),
  );
  // html message
  // text message
  $form['email_settings']['registration']['quick_donate_registration_html_email'] = array(
    '#type' => 'textarea',
    '#title' => t('HTML message'),
    '#description' => t('HTML version of the Quick Donate registration email.'),
    '#default_value' => variable_get('quick_donate_registration_html_email', ''),
  );
  $form['email_settings']['registration']['html_tokens'] = array(
    '#type' => 'fieldset',
    '#title' => t('available tokens'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['email_settings']['registration']['html_tokens']['quick_donate_registration_html_token_help'] = array(
    '#type' => 'markup',
    '#value' => theme('token_help', 'quick_donate'),
  );
  $form['email_settings']['registration']['quick_donate_registration_text_email'] = array(
    '#type' => 'textarea',
    '#title' => t('Text message'),
    '#description' => t('Plaintext version of the Quick Donate registration email.'),
    '#default_value' => variable_get('quick_donate_registration_text_email', ''),
  );
  $form['email_settings']['registration']['text_tokens'] = array(
    '#type' => 'fieldset',
    '#title' => t('available tokens'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['email_settings']['registration']['text_tokens']['quick_donate_registration_text_token_help'] = array(
    '#type' => 'markup',
    '#value' => theme('token_help', 'quick_donate'),
  );
  
  // card expiring message
  $form['email_settings']['card_expiration'] = array(
    '#type' => 'fieldset',
    '#title' => t('Quick Donate card_expiration Email'),
    '#description' => t('Configure mail template & settings when a user enrolls in Quick Donate'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['email_settings']['card_expiration']['quick_donate_card_expiration_confirmations'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Quick Donate card_expiration emails on signup'),
    '#description' => t('When enabled any user who signs up for Quick Donate will receive a card expiration email using the template settings below.'),
    '#default_value' => variable_get('quick_donate_card_expiration_confirmations', '1'),
  );
  $form['email_settings']['card_expiration']['quick_donate_card_expiration_from_address'] = array(
    '#type' => 'textfield',
    '#title' => t('From email'),
    '#description' => t(''),
    '#default_value' => variable_get('quick_donate_card_expiration_from_address', ''),
  );
  $form['email_settings']['card_expiration']['quick_donate_card_expiration_from_name'] = array(
    '#type' => 'textfield',
    '#title' => t('From name'),
    '#description' => t(''),
    '#default_value' => variable_get('quick_donate_card_expiration_from_name', ''),
  );
  $form['email_settings']['card_expiration']['quick_donate_card_expiration_bcc_address'] = array(
    '#type' => 'textfield',
    '#title' => t('BCC'),
    '#description' => t('Comma separated list of email addresses that will recieve a BCC copy of any Quick Donate card_expiration emails. (example: test@example.com,test2@example.com)'),
    '#default_value' => variable_get('quick_donate_card_expiration_bcc_address', ''),
  );
  $form['email_settings']['card_expiration']['quick_donate_card_expiration_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#description' => t('The subject line for Quick Donate card_expiration emails.'),
    '#default_value' => variable_get('quick_donate_card_expiration_subject', ''),
  );
  // html message
  // text message
  $form['email_settings']['card_expiration']['quick_donate_card_expiration_html_email'] = array(
    '#type' => 'textarea',
    '#title' => t('HTML message'),
    '#description' => t('HTML version of the Quick Donate card expiration email.'),
    '#default_value' => variable_get('quick_donate_card_expiration_html_email', ''),
  );
  $form['email_settings']['card_expiration']['html_tokens'] = array(
    '#type' => 'fieldset',
    '#title' => t('available tokens'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['email_settings']['card_expiration']['html_tokens']['quick_donate_card_expiration_html_token_help'] = array(
    '#type' => 'markup',
    '#value' => theme('token_help', 'quick_donate'),
  );
  $form['email_settings']['card_expiration']['quick_donate_card_expiration_text_email'] = array(
    '#type' => 'textarea',
    '#title' => t('Text message'),
    '#description' => t('Plaintext version of the Quick Donate card expiration email.'),
    '#default_value' => variable_get('quick_donate_card_expiration_text_email', ''),
  );
  $form['email_settings']['card_expiration']['text_tokens'] = array(
    '#type' => 'fieldset',
    '#title' => t('available tokens'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['email_settings']['card_expiration']['text_tokens']['quick_donate_card_expiration_text_token_help'] = array(
    '#type' => 'markup',
    '#value' => theme('token_help', 'quick_donate'),
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

