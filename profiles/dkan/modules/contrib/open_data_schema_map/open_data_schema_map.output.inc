<?php
/**
 * @file
 * Output formats for ODSM.
 */

/**
 * Prettify output formats array for odsm form
 */

function open_data_schema_map_form_output_formats() {
  $output_formats = array_keys(open_data_schema_map_retrieve_output_formats());
  $r = array();
  foreach ($output_formats as $output_format) {
    $r[$output_format] = $output_format;
  }
  return $r;
}

/**
 * Returns a full, single output format array
 */
function open_data_schema_map_output_format_load($name) {
  $output_formats = open_data_schema_map_retrieve_output_formats();
  if (in_array($name, array_keys($output_formats))) {
    return $output_formats[$name];
  }
  else {
    return FALSE;
  }
}

/**
 * Returns list of formats off hook_odsm_output_format implementations.
 */
function open_data_schema_map_retrieve_output_formats() {
  $output_formats = array();
  foreach (module_implements('odsm_output_format') as $module) {
    $module_outputs = $module . '_odsm_output_format';
    $module_outputs = $module_outputs();
    foreach ($module_outputs as $key => $value) {
      if (!in_array($key, array_keys($output_formats))) {
        $output_formats[$key] = $value;
      }
    }
  }
  return $output_formats;
}

/**
 * Implements hook_odsm_output_format().
 */
function open_data_schema_map_odsm_output_format() {
  return array(
    'json' => array(
      'callback' => 'open_data_schema_map_json_output',
      'headers' => array(
        'Content-Type' => 'application/json',
      ),
    ),
    'json-pretty' => array(
      'callback' => 'open_data_schema_map_json_pretty_output',
      'headers' => array(
        'Content-Type' => 'application/json',
      ),
    ),
  );
}

/**
 * Default json output.
 */
function open_data_schema_map_json_output($api, $results) {
  return drupal_json_encode($results);
}

/**
 * Alternative "pretty" json output if php -v > 5.3.
 */
function open_data_schema_map_json_pretty_output($api, $results) {
  // We are returning JSON, so tell the browser.

  if (isset($results)) {
    // The PHP version cannot change within a request.
    static $php530;
    if (!isset($php530)) {
      $php530 = version_compare(PHP_VERSION, '5.3.0', '>=');
    }
    if ($php530) {
      return json_encode($results, JSON_PRETTY_PRINT | JSON_UNESCAPED_SLASHES);
    }
    // json_encode() escapes <, >, ', &, and " using its options parameter, but
    // does not support this parameter prior to PHP 5.3.0.
    include_once DRUPAL_ROOT . '/includes/json-encode.inc';
    return drupal_json_encode_helper($results);
  }
}
