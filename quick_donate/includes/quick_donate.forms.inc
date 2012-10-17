<?php

/**
 *  @file Form arrays, validate and submit handlers
 */

/**
 * Render donation form ask amounts, sustainer checkbox, and a submit button.
 */
function quick_donate_donation_form($edit, $nid) {

  module_load_include('module', 'webform');
  module_load_include('module', 'uc_store');
  $webform_node = node_load(array('nid' => $nid));
  global $user;
  $account = $user;
  if (!isset($account->quick_donate_registered)) {
    $account = user_load(array('uid' => $account->uid));
  }
 
  $payment_methods = quick_donate_load_payment_methods($account->uid);
  if ($payment_methods) {
    $default_method = $payment_methods[0];
    $state = uc_zone_get_by_id($method['billing_zone']);
  }

  // plant user details on form.
  $form['acct_details'] = array(
    '#type' => 'markup',
    '#value' => theme('quick_donate_account_details', $default_method, $nid),
  );
  
  // track down ask component & parent fieldset.
  foreach ($webform_node->webform['components'] as $cid => $component) {
    if ($component['form_key'] == 'amount') {
      $ask_component = $component;
    }
    elseif ($component['form_key'] == 'other_amount') {
      $other_amount = $component;
    }
    elseif ($component['form_key'] == 'recurs_monthly') {
      $sustainer = $component;
    }
  }

  $components[$ask_component['cid']] = $ask_component;

  if (isset($other_amount)) {
    $components[$other_amount['cid']] = $other_amount;
  }

  if (isset($sustainer)) {
    $sustainer['pid'] = $ask_component['pid'];
    $components[$sustainer['cid']] = $sustainer;
  }

  $fieldset = array();
  
  // get parent fieldset if present.
  $components[$ask_component['pid']] = $webform_node->webform['components'][$ask_component['pid']];
  $form['submitted'] = array();
  $derp = 1;
  
  // render ask amount component and parent fieldset (if available)
  _webform_components_tree_build($components, $components, 0, $derp);
  
  // if ask amount component has a parent fieldset, render fieldset and set parent form element to the rendered fieldset.
  if (isset($ask_component['pid'])) {
    _webform_client_form_add_component($node, $components[$ask_component['pid']], $component_value, $form['submitted'], $form, $form_state, $submission, 'form', 0, FALSE);
    $target =& $form['submitted'][$components[$ask_component['pid']]['form_key']];
  }
  else {
    $target =& $form['submitted'];
  }
  
  // render ask amount component.
  _webform_client_form_add_component($node, $components[$ask_component['cid']], $component_value, $target, $form, $form_state, $submission, 'form', 0, FALSE);

  // rener other amount component if present.
  if (isset($other_amount)) {
    _webform_client_form_add_component($node, $components[$other_amount['cid']], $component_value, $target, $form, $form_state, $submission, 'form', 0, FALSE);
  }
  
  // render recurring payment component if present.
  if (isset($sustainer)) {
    _webform_client_form_add_component($node, $components[$sustainer['cid']], $component_value, $target, $form, $form_state, $submission, 'form', 0, FALSE);
  }

  if (count($payment_methods) > 1 || variable_get('quick_donate_debug_mode', 0)) {
    if (variable_get('quick_donate_debug_mode', 0)) {
      drupal_set_message(t('Quick Donate is currently in debug mode. Be sure to turn debug mode off for live sites.'));
    }
    $uid = $account->uid;
    // TODO: fiddle &destination so once user is done viewing/editing payment methods they are returned to the donation form.
    $profile_link = l(t('Manage your payment methods'), "user/$uid/payment_methods");
    foreach($payment_methods as $method) {
      $options[$method['pid']] = ucwords($method['data']['cc_data']['cc_type']) . '-' . $method['data']['cc_data']['cc_exp_month'] . '/' . $method['data']['cc_data']['cc_exp_year'];
    }

    $form['payment_method'] = array(
      '#type' => 'fieldset',
      '#title' => t('Payment methods'),
    );
    $form['payment_method']['method_select'] = array(
      '#type' => 'select',
      '#title' => t('Select a payment method'),
      '#options' => $options,
      '#default_value' => $payment_methods[0]['pid'],
      '#description' => t('Select a payment method to use for this donation. !link', array('!link' => $profile_link)),
    );
  }
  else {
    $form['method_select'] = array(
      '#type' => 'value',
      '#value' => $payment_methods[0]['pid'],
    );
  }
  $form['webform_nid'] = array(
    '#type' => 'value',
    '#value' => $nid,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Donate'),
  );
  $form['#validate'][] = 'quick_donate_validate_donation';
  $form['#submit'][] = 'quick_donate_process_donation';
  return $form;
}

function quick_donate_validate_donation(&$form, $form_state) {

  $ask_amount = $form_state['values']['amount'];
  $other_amount = $form_state['values']['other_amount'];

  if (!$ask_amount && !$other_amount) {
    form_set_error('ask_amount', t('Y U NO SELECT ASK AMOUNT?'));
  }
  elseif ($ask_amount == 'other' && (!is_numeric($other_amount) || !$other_amount)) {
    form_set_error('other_amount', t('Y U NO ENTER OTHER AMOUNT?'));
  }
}

/**
 * Process a donation form submission via drupal_execute().
 *
 * This is the heart of the donation process. This function builds a full donation form submission from a combination of submitted
 * ask amounts, user profile settings, and the user's payment method details.
 */
function quick_donate_process_donation($form, $form_state) {

    global $user;
    $account = $user;
    if (!isset($account->quick_donate_registered)) {
      $account = user_load(array('uid' => $account->uid));
    }

    $method = quick_donate_payment_method_load($form_state['values']['method_select']);
    
    $nid = $form_state['values']['webform_nid'];
    $node = node_load(array('nid' => $nid));
    $ask_amount = is_numeric($form_state['values']['amount']) ? $form_state['values']['amount'] : $form_state['values']['other_amount'];

    $submission = array(
      'webform_nid' => $nid,
      'address' => $method['billing_street1'],
      'address_line_2' => $method['billing_street2'],
      'city' => $method['billing_city'],
      'country' => $method['billing_country'],
      'state' => $method['billing_zone'] ? $method['billing_zone'] : '',
      'zip' => $method['billing_postal_code'],
      'card_number' => '***********' . $method['data']['cc_txns']['references'][$method['ref_token']]['card'], // TODO: replace this with function
      'card_expiration_month' => $method['data']['cc_data']['cc_exp_month'],
      'card_expiration_year' => $method['data']['cc_data']['cc_exp_year'],
      'card_cvv' => $method['data']['cc_data']['cc_cvv'] ? $method['data']['cc_data']['cc_cvv'] : 'xxx',
      'recurs_monthly' => $form_state['values']['recurs_monthly'],
      'amount' => $form_state['values']['amount'],
      'other_amount' => $form_state['values']['other_amount'],
      'first_name' => $method['billing_first_name'],
      'last_name' => $method['billing_last_name'],
      'email' => $account->mail,
    );

    // Start with some defaults.
    $new_state = array(
      'quick_donate_submission' => TRUE,
      'fundraiser_reference_txn' => TRUE,
      'fundraiser_reference_txn_id' => $method['ref_token'],
      'storage' => array(
        'page_num' => 1,
        'page_count' => 0,
      ),
      'clicked_button' => array(
        '#parents' => array(),
      ),
      'submitted' => 1,
      'values' => array(
        'op' => !empty($node->webform['submit_text']) ? $node->webform['submit_text'] : 'Submit',
        'submit' => 'Submit',
        'form_id' => 'webform_client_form_' . $node->nid,
        'process_input' => 1,
      ),
    );

    $fields = _quick_donate_load_webform_fields($nid);

    // Build $form_state based on available webform components.
    // Note $save is a temporary variable used to proxy portions of $form_state during the build process.
    foreach ($fields as $field) {
      
      if ($field->parent_key == NULL) {
          $save = &$new_state['values']['submitted'];
      }
      else {
        $save = &$new_state['values']['submitted'][$field->parent_key];
      }

      switch ($field->type) {
        case 'fieldset';
          $new_state['values']['submitted'][$field->form_key] = array();
          break;
        case 'hidden':
          $save[$field->form_key] = $submission[$field->form_key];
          $save[$field->cid] = $submission[$field->form_key];
          //$submission['data'][$field->cid]['value'] = array($submission[$field->form_key]);
          break;
        case 'email':
        case 'textfield':
        case 'textarea':
          $save[$field->form_key] = $submission[$field->form_key];
          break;
        case 'select':

          if ($field->form_key === 'recurs_monthly' && isset($form_state['values']['recurs_monthly'])) {

           if ($form_state['values']['recurs_monthly'] == 'recurs') {
              $save['recurs_monthly'] = 'recurs';
            }
            break;
          }
          $extra = unserialize($field->extra);
          if (!strcmp($extra['multiple'],'Y')) {
            $extra['multiple'] = 1;
          }
          if (!isset($extra['multiple']) || $extra['multiple'] == 0) {
            // Fundraiser's form_alter nerfs both of these fields. Hoop jumping is required to get past this.
            if ($field->form_key === 'card_expiration_month' || $field->form_key === 'card_expiration_year') {
              $save['expiration_date'][$field->form_key] = $submission[$field->form_key];
            }
            $save[$field->form_key] = $submission[$field->form_key];
          }
          else {
    
            $save[$field->form_key] = array();
            if (strpos($submission[$field->form_key], ',')) {
              $submission[$field->form_key] = explode(',', trim($submission[$field->form_key]));
            }
            else {
              $submission[$field->form_key] = array(trim($submission[$field->form_key]));
            }
            if (preg_match("/\n/", $extra['items'])) {
              $extra['items'] = explode("\n", $field->extra['items']);
            }
            else {
              $extra['items'] = array($extra['items']);
            }

            foreach ($extra['items'] as $item) {
              $item = preg_replace('/\|.*$/', '', $item);

              if (in_array($item, $submission[$field->form_key])) {
                $save[$field->form_key][$item] = $item;
              }
            }
          
            foreach ($submission[$field->form_key] as $item) {
              $item = trim($item);
              if (is_numeric($item)) {
                $item = (int) $item;
              }
              $save[$field->form_key][$item] = $item;
            }
          }
          break;
        case 'date':
          $date = explode('/', $submission[$field->form_key]);
          $save[$field->form_key] = array(
            'month' => $date[0],
            'day' => $date[1],
            'year' => $date[2],
          );
          break;
      }
    }

    
    $new_state['clicked_button']['#post']['submitted'] = $form_state['values']['submitted'];
    $submission = (object) $submission;

    drupal_execute('webform_client_form_' . $submission->webform_nid, $new_state, $node, $submission, FALSE, FALSE);

    //$redirect = url($new_state['redirect'][0] . "?" . $new_state['redirect'][1]);
    drupal_goto($new_state['redirect'][0], $new_state['redirect'][1]);

}


function quick_donate_register($edit, $nid, $sid) {

  // temporarily disable sid security so the form submission doesn't catch a 403 error.
  db_query('DELETE FROM {fundraiser_secure} WHERE sid=%d', $sid);
  $fundraiser_data = _quick_donate_fundraiser_order_data(array('sid' => $sid));
 
  $order_id = $fundraiser_data['order_id'];
  $order = uc_order_load($order_id);
  $data = serialize($order->data);

  $uid = $order->uid;
  $ref_token = _quick_donate_get_token_from_order($order);

  $form['nid'] = array(
    '#type' => 'value',
    '#value' => $nid,
  );
  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $uid,
  );
  $form['sid'] = array(
    '#type' => 'value',
    '#value' => $sid,
  );
  $form['order_id'] = array(
    '#type' => 'value',
    '#value' => $fundraiser_data['order_id'],
  );
  $form['gateway_id'] = array(
    '#type' => 'value',
    '#value' => $order->data['gateway'],
  );
  $form['ref_token'] = array(
    '#type' => 'value',
    '#value' => $ref_token,
  );
  $form['data'] = array(
    '#type' => 'value',
    '#value' => $data,
  );
  $form['quick_donate'] = array(
    '#type' => 'fieldset',
    '#title' => t('Register for quick donation.'),
  );
  $form['quick_donate']['quick_donate_message'] = array(
    '#type' => 'markup',
    '#value' => variable_get('quick_donate_registration_message', t('The quick donate service allows you to streamline the donation process by replacing tedious forms with a one-click donation experience.')),
    '#prefix' => '<p>',
    '#suffix' => '</p>',
  );
 /* $form['quick_donate']['remember_me'] = array(
    '#type' => 'checkbox',
    '#title' => t('Remember me on this computer.'),
    '#description' => t(''),
  );*/
  $form['quick_donate']['submit'] = array(
    '#type' => 'submit',
    '#value' => t(variable_get('quick_donate_registration_button', 'Register for quick donate')),
  );

  $form['#redirect'] = variable_get('quick_donate_thank_you_page', FALSE);
  $form['#validate'][] = 'quick_donate_validate_registration';
  $form['#submit'][] = 'quick_donate_submit_registration';
  return $form;
}

/**
 * Validation callback for confirmation page registration form.
 */
function quick_donate_validate_registration(&$form, $form_state) {

}

/**
 * Submit callback for confirmation page registration form.
 */
function quick_donate_submit_registration($form, $form_state) {

  global $user;
  $uid = $form_state['values']['uid'];
  $gateway_id = $form_state['values']['gateway_id'];
  $ref_token = $form_state['values']['ref_token'];
  $order_id = $form_state['values']['order_id'];
  $sid = $form_state['values']['sid'];
  $data = $form_state['values']['data'];
  quick_donate_register_user($uid, 1);
  quick_donate_save_payment_method($uid, $gateway_id, $ref_token, $order_id, $sid, $data);
  
  // If the user selects "remember me" we want to authenticate them.
  // This only makes sense if they aren't already logged in.
  /*if (!$user->uid && $uid && $form_state['values']['remember_me']) {
    $user = user_load($uid);
    sess_regenerate();
  }*/

  return $form['#redirect'];
 
}

function quick_donate_payment_methods($account) {
  // prompt for 2nd login
  if (!quick_donate_2nd_auth_validate()) {
    $content = drupal_get_form('quick_donate_2nd_auth', array(), $_GET['q']);
    return theme_box('Please log in to view payment methods', $content);
  }
  
  module_load_include('module', 'fundraiser');
  $payment_methods = quick_donate_load_payment_methods($account->uid);
  if (count($payment_methods)) {
    foreach ($payment_methods as $payment_method) {
      $content .= theme('quick_donate_payment_method', $payment_method);
    }
  }
  
  
  if (!fundraiser_is_secure && !variable_get('quick_donate_debug_mode', 0)) {
    $content = t('For your protection, payment methods can only be viewed over a secure connection. Please contact the site administrator.');
  }

  return theme_box('Payment Methods', $content);
  
}


function quick_donate_2nd_auth() {
  $form = array();
  global $user;
  $form['#redirect'] = $redirect;
  
  $form['lede'] = array(
    '#type' => 'markup',
    '#value' => variable_get('quick_donate_auth_lede', t('For your protection you must reauthenticate to view payment method details.')),
  );
  
  $form['username'] = array(
    '#type' => 'textfield',
    '#title' => t('user name'),
    '#size' => 10,
    '#required' => TRUE,
    '#default_value' => $user->name,
  );
  
  $form['pass'] = array(
    '#type' => 'password',
    '#title' => t('password'),
    '#size' => 10,
    '#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('log in'),
  );
  $form['#submit'][] = 'quick_donate_2nd_auth_submit';
  return $form;
}


function quick_donate_2nd_auth_submit(&$form, $form_state) {
  $uid = db_result(db_query("SELECT uid FROM {users} WHERE name ='%s' AND pass = MD5('%s')", $form_state['values']['username'], $form_state['values']['pass']));
   if ($uid) {
     $_SESSION['quick_donate_2nd_auth'] = time() + (int) variable_get('quick_donate_2nd_auth_period', 300);
   }
   else {
     form_set_error('pass', t('Your username or password is incorrect.'));
   }
}

/**
 * Payment method edit form.
 */
function quick_donate_edit_payment_method($edit, $account, $method) {
  module_load_include('module', 'fundraiser');
  module_load_include('inc', 'fundraiser', 'fundraiser.sustainer.forms');

  $uid = $account->uid;

  // Immediately bail out if page is not secure
  if (!fundraiser_is_secure && !variable_get('quick_donate_debug_mode', 0)) {
    $pid = $method['pid'];
    $content = t('For your protection, payment methods can only be viewed over a secure connection. Please contact the site administrator.');
    watchdog('quick_donate', t('Error: %user attempted to edit a payment method via an insecure (HTTP) connection.', array('%user' => $account->name)), array(), WATCHDOG_ERROR, l(t('view'), "user/$uid/payment_methods/$pid/edit"));
    $form['error'] = array(
      '#type' => 'markup',
      '#value' => "<p>$content</p>",
    );
    return $form;
  }
  
  // prompt 2nd login if login is expired
  if (!quick_donate_2nd_auth_validate()) {
    $form = quick_donate_2nd_auth();
    return $form;
  }

  $month_options = array(
    '1' => t('January'),
    '2' => t('February'),
    '3' => t('March'),
    '4' => t('April'),
    '5' => t('May'),
    '6' => t('June'),
    '7' => t('July'),
    '8' => t('August'),
    '9' => t('September'),
    '10' => t('October'),
    '11' => t('November'),
    '12' => t('December'),
  );
  $year = date('Y');
  for ($i = 0; $i < 10; ++$i) {
    $year_options[$year + $i] = $year + $i;
  }

  $countries = uc_country_select(uc_get_field_name('country'), 840, NULL, 'name', uc_address_field_required('country'));
  $zones = uc_zone_select(uc_get_field_name('zone'), $arg1->billing_zone, NULL, 840, 'name', uc_address_field_required('zone'));
  $form['pid'] = array(
    '#type' => 'value',
    '#value' => $method['pid'],
  );
  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $uid,
  );
  $form['gateway_type'] = array(
    '#type' => 'value',
    '#value' => $method['gateway_type'],
  );
  $form['gateway_id'] = array(
    '#type' => 'value',
    '#value' => $method['gateway_id'],
  );
  $form['ref_id'] = array(
    '#type' => 'value',
    '#value' => $method['ref_token'],
  );
  $form['order_id'] = array(
    '#type' => 'value',
    '#value' => $method['order_id'],
  );
  $form['billing_first_name'] = array(
    '#type' => 'textfield',
    '#title' => t('First name'),
    '#description' => t(''),
    '#size' => 20,
    '#required' => TRUE,
    '#default_value' => $method['billing_first_name'],
  );
  $form['billing_last_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Last Name'),
    '#description' => t(''),
    '#size' => 20,
    '#required' => TRUE,
    '#default_value' => $method['billing_last_name'],
  );
  $form['billing_street1'] = array(
    '#type' => 'textfield',
    '#title' => t('Address 1'),
    '#description' => t(''),
    '#required' => TRUE,
    '#default_value' => $method['billing_street1'],
  );
  $form['billing_street2'] = array(
    '#type' => 'textfield',
    '#title' => t('Address 2'),
    '#description' => t('Apt or suite #'),
    '#default_value' => $method['billing_street2'],
  );
  $form['billing_city'] = array(
    '#type' => 'textfield',
    '#title' => t('City'),
    '#description' => t(''),
    '#size' => 30,
    '#required' => TRUE,
    '#default_value' => $method['billing_city'],
  );
  $form['billing_zone'] = array(
    '#type' => 'select',
  	'#title' => 'State',
    '#options' => $zones['#options'],
    '#required' => TRUE,
  	'#default_value' => $method['billing_zone'],
  );
  $form['billing_country'] = array(
    '#type' => 'select',
  	'#title' => 'Country',
    '#options' => $countries['#options'],
    '#required' => TRUE,
  	'#default_value' => $method['billing_country'],
  );
  $form['billing_postal_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Zipcode'),
    '#description' => t(''),
    '#size' => 10,
    '#required' => TRUE,
    '#default_value' => $method['billing_postal_code'],
  );
  $form['cc_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Credit card number'),
    '#description' => t(''),
    '#required' => TRUE,
  );
  $form['cc_exp'] = array(
    '#type' => 'fieldset',
    '#title' => t('Expiration date'),
  );
  $form['cc_exp']['exp_month'] = array(
    '#type' => 'select',
    '#title' => t(''),
    '#options' => $month_options,
    '#required' => TRUE,
  );
  $form['cc_exp']['exp_year'] = array(
    '#type' => 'select',
    '#title' => t(''),
    '#options' => $year_options,
    '#required' => TRUE,
  );
  $form['cc_cvv'] = array(
    '#type' => 'textfield',
    '#title' => t('CVV'),
    '#description' => t('This is the three digit code on the back of your card'),
    '#size' => 3,
    '#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['#redirect'] = "user/$uid/payment_methods";
  $form['#validate'][] = 'quick_donate_validate_edit_payment_method';
  $form['#submit'][] = 'quick_donate_submit_edit_payment_method';

  return $form;
}


/**
 * Validation handler for edit payment method form.
 */
function quick_donate_validate_edit_payment_method(&$form, &$form_state) {

  $values = $form_state['values'];
  // validate credit card information
  $cc_number = $values['cc_number'];
  $cc_expiration_month =  $values['exp_month'];
  $cc_expiration_year = $values['exp_year'];
  $cc_cvv = $values['cc_cvv'];
  
  //validate cc number
  if ((variable_get('uc_credit_validate_numbers', TRUE) && !_valid_card_number($cc_number)) || !ctype_digit($cc_number)) {
    form_set_error('cc_number',"You have entered an invalid credit card number.");
  }

  // Validate the card expiration date.
  if (!_valid_card_expiration($cc_expiration_month, $cc_expiration_year)) {
    form_set_error('cc_exp][card_exp_date',t('The credit card you entered has expired.'));
  }
  else {
    $form_state['values']['cc_exp_date'] = array(
      'day' => 1,
      'month' => $values['exp_month'],
      'year' => $values['exp_year'],
    );
  }

  // Validate the CVV Number
  if (variable_get('uc_credit_cvv_enabled', TRUE) && !_valid_cvv($cc_cvv)) {
    form_set_error('cc_cvv',t('You have entered an invalid CVV number.'));
  }
}


/**
 * Submit handler for payment method edit form.
 */
function quick_donate_submit_edit_payment_method($form, $form_state) {
  module_load_include('inc', 'fundraiser', 'fundraiser_billing');
  $values = $form_state['values'];

  //$payment_details = _fundraiser_get_payment_details($values['order_id']);

  $billing_info = array(
    'master_order_id' =>  $values['order_id'],
    'first_name' => $values['billing_first_name'],
    'last_name' => $values['billing_last_name'],
    'email' => $values['email'],
    'address_1' => $values['billing_street1'],
    'address_2' => $values['billing_street2'],
    'city' => $values['billing_city'],
    'country' => $values['billing_country'],
    'state' => $values['billing_zone'],
    'zipcode' => $values['billing_postal_code'],
    'card_num' => $values['cc_number'],
    'card_exp_date' => $values['cc_exp_date'],
    'card_cvv' => $values['cc_cvv'],
  );
  $ret = fundraiser_update_billing_info($values['gateway_id'], $values['ref_id'], $billing_info);
  if (isset($ret['success']) && $ret['success'] == 1) {
    quick_donate_update_payment_method($values['pid'], $billing_info);
    drupal_set_message(t('Your payment details have been updated'));
    return $form['#redirect'];
  }
  else {
    drupal_set_message(t('There was a problem updating your payment method details. Please contact the site administrator.'));
  }
}


function quick_donate_delete_payment_method_form($edit, $account, $method) {
  
  $form = array();
  
  // Immediately bail out if page is not secure
  if (!fundraiser_is_secure && !variable_get('quick_donate_debug_mode', 0)) {
    $uid = $account->uid;
    $pid = $method['pid'];
    $content = t('For your protection, payment methods can only be viewed over a secure connection. Please contact the site administrator.');
    watchdog('quick_donate', t('Error: %user attempted to delete a payment method via an insecure (HTTP) connection.', array('%user' => $account->name)), array(), WATCHDOG_ERROR, l(t('view'), "user/$uid/payment_methods/$pid/delete"));
    $form['error'] = array(
      '#type' => 'markup',
      '#value' => "<p>$content</p>",
    );
    return $form;
  }
  
  // prompt 2nd login if login is expired
  if (!quick_donate_2nd_auth_validate()) {
    $form = quick_donate_2nd_auth();
    return $form;
  }
  
  $box_title = t('Are you sure you want to delete this payment method?');
  $box_message = t('This action cannot be undone. Are you sure you want to proceed?');
  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $account->uid,
  );
  $form['pid'] = array(
    '#type' => 'value',
    '#value' => $method['pid'],
  );
  $form['prefix'] = array(
    '#type' => 'markup',
    '#value' => theme_box($box_title, $box_message),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
  );
  $form['cancel'] = array(
    '#type' => 'button',
    '#value' => t('Cancel'),
    '#executes_submit_callback' => TRUE,
    '#submit' => array('quick_donate_cancel_submission'),
  );
  $form['#submit'][] = 'quick_donate_delete_payment_method_submit';
  return $form;
}

/**
 * Cancel button submit callback. Bails to user profile payment method
 * screen if the user cancels current operation.
 */
function quick_donate_cancel_submission($form, $form_state) {
  $uid = $form_state['values']['uid'];
  drupal_goto("user/$uid/payment_methods");
}

function quick_donate_delete_payment_method_submit($form, $form_state) {
  $uid = $form_state['values']['uid'];
  $pid = $form_state['values']['pid'];
  quick_donate_delete_payment_method($pid, $uid);
  drupal_set_message('Payment method has been deleted.');
  $redirect = $methods ? "user/$uid/payment_methods" : "user/$uid";
  drupal_goto($redirect);
}
