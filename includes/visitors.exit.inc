<?php

/**
 * Verify the syntax of the given ip address.
 *
 * @param ip
 *   A string containing an ip address.
 * @return
 *   TRUE if the ip is in a valid format, FALSE on failure.
 */
function visitors_is_ip_valid($ip) {
  $result = preg_match('/^(\d{1,3})\.(\d{1,3})\.(\d{1,3})\.(\d{1,3})$/',
                        $ip,
                        $matches
                      );

  return (
    $result &&
    isset($matches[0]) &&
    ($matches[0] === $ip) &&
    ($matches[1] >= 1) && ($matches[1] <= 255) &&
    ($matches[2] >= 0) && ($matches[2] <= 255) &&
    ($matches[3] >= 0) && ($matches[3] <= 255) &&
    ($matches[4] >= 0) && ($matches[4] <= 255)
  );
}

/**
 * Get visitors ip address.
 *
 * @return
 *   A string containing an ip address ('0.0.0.0' on failure).
 */
function visitors_get_ip() {
  if (isset($_SERVER['HTTP_X_FORWARDED_FOR'])) {
    $ip_array = explode(',', $_SERVER['HTTP_X_FORWARDED_FOR']);
    $ip       = trim(reset($ip_array));
  }
  else {
    $ip = $_SERVER['REMOTE_ADDR'];
  }

  return (visitors_is_ip_valid($ip) ? $ip : '0.0.0.0');
}

/**
 * Converts a string containing an visitors (IPv4) Internet Protocol dotted
 * address into a proper address.
 *
 * @return
 *   string
 */
function visitors_get_ip_str() {
  return sprintf("%u", ip2long(visitors_get_ip()));
}

/**
 * Get full path request uri.
 *
 * @return
 *   string full path
 */
function visitors_get_url() {
  return
    urldecode(sprintf('http://%s%s', $_SERVER['HTTP_HOST'], request_uri()));
}

/**
 * Get internal path.
 *
 * @return
 *   string internal path
 */
function visitors_get_path() {
  return $_GET['q'];
}

/**
 * Get the address of the page (if any) which referred the user agent to the
 * current page.
 *
 * @return
 *   string referer, or empty string if referer does not exist
 */
function visitors_get_referer() {
  $referer = '';

  if (isset($_SERVER['HTTP_REFERER'])) {
    if (preg_match('/^(?:[;\/?:@&=+$,]|(?:[^\W_]|[-_.!~*\()\[\] ])|(?:%[\da-fA-F]{2}))*$/', $_SERVER['HTTP_REFERER'])) {
      $referer = rawurldecode($_SERVER['HTTP_REFERER']);
    }
  }

  return $referer;
}

/**
 * Get the title of the current page.
 *
 * @return
 *   string title of the current page
 */
function visitors_get_title() {
  return htmlspecialchars_decode(backdrop_get_title(), ENT_QUOTES);
}

/**
 * Get visitor user agent.
 *
 * @return
 *   string user agent, or empty string if user agent does not exist
 */
function visitors_get_user_agent() {
  return isset($_SERVER['HTTP_USER_AGENT']) ? $_SERVER['HTTP_USER_AGENT'] : '';
}

/**
 * Retrieve geoip data for ip.
 *
 * @param ip
 *   A string containing an ip address.
 * @return
 *   array geoip data array.
 */
function visitors_get_geoip_data($ip) {
  $result = array(
    'continent_code' => '',
    'country_code'   => '',
    'country_code3'  => '',
    'country_name'   => '',
    'region'         => '',
    'city'           => '',
    'postal_code'    => '',
    'latitude'       => '0',
    'longitude'      => '0',
    'dma_code'       => '0',
    'area_code'      => '0'
  );

  if (function_exists('geoip_record_by_name')) {
    $data = @geoip_record_by_name($ip);
    if ((!is_null($data)) && ($data !== FALSE)) {
      /* Transform city value from iso-8859-1 into the utf8. */
      $data['city'] = utf8_encode($data['city']);

      $result = $data;
    }
  }

  return $result;
}

/**
 * Implements hook_exit().
 */
function visitors_exit() {
  backdrop_bootstrap(BACKDROP_BOOTSTRAP_SESSION);
#  backdrop_load('module', 'user');

  global $user;
  $loadedUser = user_load($user->uid);
  $not_admin = user_has_role('administrator', $loadedUser);
  $log_admin = config_get('visitors.settings', 'visitors_exclude_administer_users');

  if ($log_admin || $not_admin) {
    $ip_str = visitors_get_ip_str();

    $fields = array(
      'visitors_uid'            => $user->uid,
      'visitors_ip'             => $ip_str,
      'visitors_date_time'      => time(),
      'visitors_url'            => visitors_get_url(),
      'visitors_referer'        => visitors_get_referer(),
      'visitors_path'           => visitors_get_path(),
      'visitors_title'          => visitors_get_title(),
      'visitors_user_agent'     => visitors_get_user_agent()
    );

    if (module_exists('visitors_geoip')) {
      $geoip_data = visitors_get_geoip_data($ip_str);

      $fields['visitors_continent_code'] = $geoip_data['continent_code'];
      $fields['visitors_country_code']   = $geoip_data['country_code'];
      $fields['visitors_country_code3']  = $geoip_data['country_code3'];
      $fields['visitors_country_name']   = $geoip_data['country_name'];
      $fields['visitors_region']         = $geoip_data['region'];
      $fields['visitors_city']           = $geoip_data['city'];
      $fields['visitors_postal_code']    = $geoip_data['postal_code'];
      $fields['visitors_latitude']       = $geoip_data['latitude'];
      $fields['visitors_longitude']      = $geoip_data['longitude'];
      $fields['visitors_dma_code']       = $geoip_data['dma_code'];
      $fields['visitors_area_code']      = $geoip_data['area_code'];
    }

    db_insert('visitors')
      ->fields($fields)
      ->execute();
  }
}

