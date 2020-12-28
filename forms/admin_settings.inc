<?php

/**
 * @file
 * Settings page callback file for the visitors module.
 */

/**
 * Create visitors settings form.
 *
 * @return
 *   array form structure
 */
function visitors_admin_settings() {
  $form = array();
  $form['#config'] = 'visitors.settings';
  $config = config('visitors.settings');
  // only administrators can access this function

  // Generate the form - settings applying to all patterns first
  $form['visitors_settings'] = array(
    '#type' => 'fieldset',
    '#weight' => -30,
    '#title' => t('Visitors block'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('Visitors block settings')
  );

  $form['visitors_settings']['visitors_show_total_visitors'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Total Visitors'),
    '#default_value' => $config->get('visitors_show_total_visitors'),
    '#description' => t('Show Total Visitors.')
  );
  $form['visitors_settings']['visitors_start_count_total_visitors'] = array(
    '#type' => 'textfield',
    '#title' => t('Total visitors start count'),
    '#default_value' => $config->get('visitors_start_count_total_visitors'),
    '#description' => t('Start the count of the total visitors at this number. Useful for including the known number of visitors in the past.')
  );

  $form['visitors_settings']['visitors_show_unique_visitor'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Unique Visitors'),
    '#default_value' => $config->get('visitors_show_unique_visitor'),
    '#description' => t('Show Unique Visitors based on their IP.')
  );

  $form['visitors_settings']['visitors_registered_user'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Registered Users'),
    '#default_value' => $config->get('visitors_registered_user'),
    '#description' => t('Show Registered Users.')
  );

  $form['visitors_settings']['visitors_last_registered_user'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Last Registered User'),
    '#default_value' => $config->get('visitors_last_registered_user'),
    '#description' => t('Show Last Registered User.')
  );

  $form['visitors_settings']['visitors_published_nodes'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Published Nodes'),
    '#default_value' => $config->get('visitors_published_nodes'),
    '#description' => t('Show Published Nodes.')
  );

  $form['visitors_settings']['visitors_user_ip'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show User IP'),
    '#default_value' => $config->get('visitors_user_ip'),
    '#description' => t('Show User IP.')
  );

  $form['visitors_settings']['visitors_since_date'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Since Date'),
    '#default_value' => $config->get('visitors_since_date'),
    '#description' => t('Show Since Date.')
  );

  // Statistics settings.
  $form['visitors_statistics'] = array(
    '#type' => 'fieldset',
    '#title' => t('Visitors statistics'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('Visitors statistics settings')
  );

  $form['visitors_statistics']['visitors_exclude_administer_users'] = array(
    '#type' => 'checkbox',
    '#title' => t('Exclude administer users from statistics'),
    '#default_value' => $config->get('visitors_exclude_administer_users'),
    '#description' => t('Exclude hits of administer users from statistics.')
  );

  $form['visitors_statistics']['visitors_lines_per_page'] = array(
    '#type' => 'select',
    '#title' => 'Lines per page',
    '#default_value' => $config->get('visitors_lines_per_page'),
    '#options' => array(
      5 => 5, 10 => 10, 25 => 25, 50 => 50, 100 => 100, 200 => 200, 250 => 250,
      500 => 500, 1000 => 1000
    ),
    '#description' =>
      t('The default maximum number of lines to display per page.'),
  );

  $period = backdrop_map_assoc(
    array(3600, 10800, 21600, 32400, 43200, 86400, 172800, 259200, 604800,
      1209600, 2419200, 4838400, 9676800, 31536000),
      'format_interval'
    );
  $period[0] = t('Never');

  $form['visitors_statistics']['visitors_flush_log_timer'] = array(
    '#type' => 'select',
    '#title' => t('Discard visitors logs older than'),
    '#default_value'   => $config->get('visitors_flush_log_timer'),
    '#options' => $period,
    '#description' =>
      t('Older visitors log entries (including referrer statistics) will be ' .
        'automatically discarded. (Requires a correctly configured ' .
        '<a href="@cron">cron maintenance task</a>.)',
        array('@cron' => url('admin/reports/status'))
      )
  );

  return system_settings_form($form);
}

