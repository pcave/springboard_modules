<?php

/**
 * @file springboard_advocacy.views.inc
 */

/**
 * Implements hook_views_data().
 */
function springboard_advocacy_views_data() {
  $data = array();

  // Base table definition.
  $data['advocacy']['table'] = array(
    'group' => t('Advocacy'),
    'base' => array(
      'title' => t('Advocacy'),
      'help' => t(''),
      'query class' => 'springboard_advocacy_views_plugin_query',
    ),
  );

  // ID
  $data['advocacy']['id'] = array(
    'title' => t('Target ID'),
    'help' => t('Target ID'),
    'field' => array(
      'handler' => 'springboard_advocacy_views_handler_field',
    ),
  );

  // Salutation
  $data['advocacy']['salutation'] = array(
    'title' => t('Salutation'),
    'help' => t('Target Salutation'),
    'field' => array(
      'handler' => 'springboard_advocacy_views_handler_field',
    ),
     'filter' => array(
       'handler' => 'springboard_advocacy_views_handler_filter_string',
    ),
  );

  // Last Name
  $data['advocacy']['first_name'] = array(
    'title' => t('First Name'),
    'help' => t('Target First Name'),
    'field' => array(
      'handler' => 'springboard_advocacy_views_handler_field',
    ),
     'filter' => array(
       'handler' => 'springboard_advocacy_views_handler_filter_string',
    ),
  );

  // Last Name
  $data['advocacy']['last_name'] = array(
    'title' => t('Last Name'),
    'help' => t('Target Last Name'),
    'field' => array(
      'handler' => 'springboard_advocacy_views_handler_field',
    ),
     'filter' => array(
       'handler' => 'springboard_advocacy_views_handler_filter_string',
    ),
  );

  // Email
  $data['advocacy']['email'] = array(
    'title' => t('Email'),
    'help' => t('Target Email'),
    'field' => array(
      'handler' => 'springboard_advocacy_views_handler_field',
    ),
     'filter' => array(
       'handler' => 'springboard_advocacy_views_handler_filter_string',
    ),
  );


  // Party
  $data['advocacy']['party'] = array(
    'title' => t('Politcal Party'),
    'help' => t('Target Party'),
    'field' => array(
      'handler' => 'springboard_advocacy_views_handler_field',
    ),
     'filter' => array(
       'handler' => 'springboard_advocacy_views_handler_filter_string',
    ),
  );

  $data['advocacy']['state'] = array(
    'title' => t('State'),
    'help' => t('Target State'),
    'field' => array(
      'handler' => 'springboard_advocacy_views_handler_field',
    ),
     'filter' => array(
       'handler' => 'springboard_advocacy_views_handler_filter_string',
    ),
  );

  $data['advocacy']['role'] = array(
    'title' => t('Role'),
    'help' => t('Target Role'),
    'field' => array(
      'handler' => 'springboard_advocacy_views_handler_field',
    ),
     'filter' => array(
       'handler' => 'springboard_advocacy_views_handler_filter_string',
    ),
  );

  $data['advocacy']['combine'] = array(
   'title' => t('Search Multiple Target Fields by Keywords'),
    'help' => t('Combine multiple fields together and search targets by them.'),
    'filter' => array(
      'handler' => 'springboard_advocacy_views_handler_filter_combine',
    ),
  );

  $data['advocacy']['search_types'] = array(
   'title' => t('Search By Type'),
    'help' => t('Adds a filter to limit searches by Custom Target, Legislator or Group'),
    'filter' => array(
      'handler' => 'springboard_advocacy_views_handler_filter_search_types',
    ),
  );

  return $data;
}

/**
 * Implements hook_views_plugins().
 */
function springboard_advocacy_views_plugins() {
  $plugins = array(
    'query' => array(
      'springboard_advocacy_views_plugin_query' => array(
        'title' => t('Springboard Advocacy APIRequest'),
        'help' => t('Uses springboard_advocacyAPIRequest for querying Targets'),
        'handler' => 'springboard_advocacy_views_plugin_query',
        'path' => drupal_get_path('module', 'springboard_advocacy') . '/includes/views/plugins',
      ),
    ),
  );

  return $plugins;
}
