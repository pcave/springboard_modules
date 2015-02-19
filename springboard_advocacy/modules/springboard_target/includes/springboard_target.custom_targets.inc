<?php

/**
 * @file
 * Form definition for Target creation and editing.
 */

/**
 * Implements hook_form
 *
 * Serves a dual function as target add and edit form
 *
 * @param null $target_id
 * @return array
 */
function springboard_target_form($target_id = NULL) {
  $form = array();

  if (isset($target_id) && is_numeric($target_id)) {
    $loader = new AdvocacyApiClientLoader();
    $client = $loader->getClient();
    $target = $client->getCustomTarget($target_id);
    if (isset($target->error)) {
      $form = array();
      $form['not_found']['#markup'] = $target->error->message;
      return $form;
    }
    $target = $target->data;
  }

  $form['row_one'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array(
        'edit-target-row-one',
      ),
    ),
    '#weight' => -5,
  );

  $form['row_one']['salutation'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => isset($target->salutation) ? $target->salutation : '',
    '#size' => 10,
    '#maxlength' => 128,
    '#attributes' => array('class' => array('target-add', 'target-salutation')),
  );

  $form['row_one']['first_name'] = array(
    '#type' => 'textfield',
    '#title' => t('First Name'),
    '#default_value' => isset($target->first_name) ? $target->first_name : '',
    '#size' => 30,
    '#maxlength' => 128,
    '#required' => TRUE,
    '#attributes' => array('class' => array('target-add', 'target-first-name')),
  );

  $form['row_one']['last_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Last Name'),
    '#default_value' => isset($target->last_name) ? $target->last_name : '',
    '#size' => 30,
    '#maxlength' => 128,
    '#required' => TRUE,
    '#attributes' => array('class' => array('target-add', 'target-last-name')),
  );

  $form['row_one']['organization'] = array(
    '#type' => 'textfield',
    '#title' => t('Organization/Affiliation'),
    '#default_value' => isset($target->organization) ? $target->organization : '',
    '#size' => 60,
    '#maxlength' => 128,
    '#attributes' => array('class' => array('target-add', 'target-organization')),
  );

  // $form['row_one']['target_type'] = array(
  //   '#type' => 'select',
  //   '#title' => t('Target Type'),
  //   '#default_value' => isset($target->target_type) ? $target->target_type : '',
  //   '#options' => springboard_target_types(),
  //   '#required' => TRUE,
  //   '#attributes' => array('class' => array('target-add', 'target-target-type')),
  // );

//  $form['row_one']['party'] = array(
//    '#type' => 'select',
//    '#title' => t('Party (If applicable)'),
//    '#default_value' => isset($target->party) ? array($target->party) : '',
//    '#options' => springboard_target_parties(),
//    '#attributes' => array('class' => array('target-add', 'target-party')),
//  );

  $form['row_one']['gender'] = array(
    '#type' => 'select',
    '#title' => t('Gender'),
    '#default_value' => isset($target->gender) ? array($target->gender) : '',
    '#options' => array('M' => 'Male', 'F' => 'Female', 'O' => 'Not Specified'), 
    '#attributes' => array('class' => array('target-add', 'target-first-name')),
  );

  $form['row_two'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array(
        'edit-target-row-two',
      ),
    ),
    '#weight' => -4,
  );

  $form['row_two']['area_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Phone Area code'),
    '#default_value' => isset($target->addresses[0]->area_code) ? $target->addresses[0]->area_code : '',
    '#size' => 10,
    '#maxlength' => 3,
    '#attributes' => array('class' => array('target-add', 'target-phone-area-code')),
  );

  $form['row_two']['phone'] = array(
    '#type' => 'textfield',
    '#title' => t('Phone'),
    '#default_value' => isset($target->addresses[0]->phone) ? $target->addresses[0]->phone : '',
    '#size' => 20,
    '#maxlength' => 8,
    '#attributes' => array('class' => array('target-add', 'target-phone')),
  );

  $form['row_two']['ext'] = array(
    '#type' => 'textfield',
    '#title' => t('Extension'),
    '#default_value' => isset($target->addresses[0]->ext) ? $target->addresses[0]->ext : '',
    '#size' => 10,
    '#maxlength' => 6,
    '#attributes' => array('class' => array('target-add', 'target-phone-extension')),
  );

  $form['row_two']['fax_area_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Fax Area Code'),
    '#default_value' => isset($target->addresses[0]->fax_area_code) ? $target->addresses[0]->fax_area_code : '',
    '#size' => 10,
    '#maxlength' => 3,
    '#attributes' => array('class' => array('target-add', 'target-fax-area-code')),
  );

  $form['row_two']['fax'] = array(
    '#type' => 'textfield',
    '#title' => t('Fax'),
    '#default_value' => isset($target->addresses[0]->fax) ? $target->addresses[0]->fax : '',
    '#size' => 20,
    '#maxlength' => 8,
    '#attributes' => array('class' => array('target-add', 'target-fax')),
  );

  $form['row_three'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array(
        'edit-target-row-three',
      ),
    ),
    '#weight' => -3,
  );
  $form['row_three']['street'] = array(
    '#type' => 'textfield',
    '#title' => t('Address'),
    '#default_value' => isset($target->addresses[0]->street) ? $target->addresses[0]->street : '',
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
    '#attributes' => array('class' => array('target-add', 'target-address')),
  );

  $form['row_three']['street_2'] = array(
    '#type' => 'textfield',
    '#title' => t('Address Line 2'),
    '#default_value' => isset($target->addresses[0]->street_2) ? $target->addresses[0]->street_2 : '',
    '#size' => 60,
    '#maxlength' => 128,
    '#attributes' => array('class' => array('target-add', 'target-address-2')),
  );

  $form['row_three']['city'] = array(
    '#type' => 'textfield',
    '#title' => t('City'),
    '#default_value' => isset($target->addresses[0]->city) ? $target->addresses[0]->city : '',
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
    '#attributes' => array('class' => array('target-add', 'target-city')),
  );

  $form['row_three']['state'] = array(
    '#type' => 'select',
    '#title' => t('State'),
    '#default_value' => isset($target->addresses[0]->state) ? array($target->addresses[0]->state) : '',
    '#options' => springboard_target_states(),
    '#required' => TRUE,
    '#attributes' => array('class' => array('target-add', 'target-state')),
  );

  $form['row_three']['zip_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Zip Code'),
    '#default_value' => isset($target->addresses[0]->zip_code) ? $target->addresses[0]->zip_code : '',
    '#size' => 10,
    '#maxlength' => 10,
    '#required' => TRUE,
    '#attributes' => array('class' => array('target-add', 'target-zip')),
  );

  $form['row_four'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array(
        'edit-target-row-four',
      ),
    ),
    '#weight' => -2,
  );

  $form['row_four']['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email Address'),
    '#default_value' => isset($target->email) ? $target->email : '',
    '#size' => 60,
    '#maxlength' => 128,
    '#attributes' => array('class' => array('target-add', 'target-email')),
  );

  $form['row_four']['facebook'] = array(
    '#type' => 'textfield',
    '#title' => t('Facebook'),
    '#default_value' => isset($target->facebook) ? $target->facebook : '',
    '#size' => 60,
    '#maxlength' => 128,
    '#attributes' => array('class' => array('target-add', 'target-facebook')),
  );

  $form['row_four']['twitter'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter'),
    '#default_value' => isset($target->twitter) ? $target->twitter : '',
    '#size' => 60,
    '#maxlength' => 128,
    '#attributes' => array('class' => array('target-add', 'target-twitter')),
  );

  $form['row_four']['website'] = array(
    '#type' => 'textfield',
    '#title' => t('Web site'),
    '#default_value' => isset($target->website) ? $target->website : '',
    '#size' => 60,
    '#maxlength' => 128,
    '#attributes' => array('class' => array('target-add', 'target-website')),
  );

  $form['row_four']['class_name'] = array(
    '#type' => 'hidden',
    '#value' => 'Target'
  );

  return $form;
}

/*
 * Return an array of target types.
 * This could become user-configurable
 */
function springboard_target_types() {
  $types = array('legislator' => 'Elected Offical', 'target' => 'Custom');
  return $types;
}

/*
 * Return an array of target parties.
 */
function springboard_target_parties() {
  $parties = array('D' => 'Democrat', 'R' => 'Republican', 'I' => 'Independent', 'O' => 'Other');
  return $parties;
}

/**
 * List states from target edit form.
 * @return array
 */
function springboard_target_states() {
  $states = array(
    'AL' => 'Alabama',
    'AK' => 'Alaska',
    'AZ' => 'Arizona',
    'AR' => 'Arkansas',
    'CA' => 'California',
    'CO' => 'Colorado',
    'CT' => 'Connecticut',
    'DE' => 'Delaware',
    'DC' => 'District Of Columbia',
    'FL' => 'Florida',
    'GA' => 'Georgia',
    'HI' => 'Hawaii',
    'ID' => 'Idaho',
    'IL' => 'Illinois',
    'IN' => 'Indiana',
    'IA' => 'Iowa',
    'KS' => 'Kansas',
    'KY' => 'Kentucky',
    'LA' => 'Louisiana',
    'ME' => 'Maine',
    'MD' => 'Maryland',
    'MA' => 'Massachusetts',
    'MI' => 'Michigan',
    'MN' => 'Minnesota',
    'MS' => 'Mississippi',
    'MO' => 'Missouri',
    'MT' => 'Montana',
    'NE' => 'Nebraska',
    'NV' => 'Nevada',
    'NH' => 'New Hampshire',
    'NJ' => 'New Jersey',
    'NM' => 'New Mexico',
    'NY' => 'New York',
    'NC' => 'North Carolina',
    'ND' => 'North Dakota',
    'OH' => 'Ohio',
    'OK' => 'Oklahoma',
    'OR' => 'Oregon',
    'PA' => 'Pennsylvania',
    'RI' => 'Rhode Island',
    'SC' => 'South Carolina',
    'SD' => 'South Dakota',
    'TN' => 'Tennessee',
    'TX' => 'Texas',
    'UT' => 'Utah',
    'VT' => 'Vermont',
    'VA' => 'Virginia',
    'WA' => 'Washington',
    'WV' => 'West Virginia',
    'WI' => 'Wisconsin',
    'WY' => 'Wyoming',
  );
  return $states;
}

/**
 * Target edit form validation.
 * @param $form
 * @param $form_state
 */
function springboard_target_form_validate($form, &$form_state) {

  if (!empty($form_state['values']['email'])) {
    $valid = valid_email_address($form_state['values']['email']);
    if (!$valid) {
      form_set_error('email', t('Invalid email address.'));
    }
  }

  if (!empty($form_state['values']['facebook'])) {
    $valid = valid_url($form_state['values']['facebook']);
    if (!$valid) {
      form_set_error('facebook', t('Invalid facebook url.'));
    }
  }

  if (!empty($form_state['values']['website'])) {
    $valid = valid_url($form_state['values']['website']);
    if (!$valid) {
      form_set_error('website', t('Invalid web site url.'));
    }
  }

  if (!empty($form_state['values']['twitter'])) {
    $valid = valid_url($form_state['values']['twitter']);
    if (!$valid) {
      form_set_error('twitter', t('Invalid Twitter url.'));
    }
  }

  if (!empty($form_state['values']['zip_code'])) {
     if (!preg_match("/^\d{5}([\-]?\d{4})?$/i", $form_state['values']['zip_code'])){
      form_set_error('zip_code', t('Invalid zip code.'));
    }
  }

}