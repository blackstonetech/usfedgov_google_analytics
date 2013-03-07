<?php

/**
 * @file
 * Drush integration for Federal Government Google Analytics to download the
 * required js library from https://github.com/GSA/DAP-Gov-wide-GA-Code.
 */

/**
 * The GSA Google Analytics js repo URI.
 */
define('FEDANALYTICS_DOWNLOAD_URI', 'https://github.com/GSA/DAP-Gov-wide-GA-Code/archive/master.zip');

/**
 * Implements of hook_drush_command().
 *
 * In this hook, you specify which commands your
 * drush module makes available, what it does and
 * description.
 *
 * Notice how this structure closely resembles how
 * you define menu hooks.
 *
 * See `drush topic docs-commands` for a list of recognized keys.
 *
 * @return
 *   An associative array describing your command(s).
 */
function usfedgov_google_analytics_libs_drush_command() {
  $items = array();

  // The key in the $items array is the name of the command.
  $items['fed_analytics_libs'] = array(
    'callback' => 'drush_fed_analytics_libs',
    'description' => dt('Download and install the federal government Google Analytics library.'),
    // No bootstrap.
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'arguments' => array(
      'path' => dt('Optional. A path to install the federal government Google Analytics library. If omitted Drush will use the default location.'),
    ),
    'aliases' => array('fedanalyticslibs'),
  );

  return $items;
}

/**
 * Implements of hook_drush_help().
 *
 * This function is called whenever a drush user calls
 * 'drush help <name-of-your-command>'
 *
 * @param
 *   A string with the help section (prepend with 'drush:')
 *
 * @return
 *   A string with the help text for your command.
 */
function usfedgov_google_analytics_libs_drush_help($section) {
  switch ($section) {
    case 'drush:fed_analytics_libs':
      return dt('Download and install the Federal Government Google Analytics library from github, default location is sites/all/libraries.');
  }
}

/**
 * Command to download the Federal Government Google Analytics library.
 */
function drush_fed_analytics_libs() {
  $args = func_get_args();
  if (!empty($args[0])) {
    $path = $args[0];
  }
  else {
    $path = 'sites/all/libraries';
  }

  // Create the path if it does not exist.
  if (!is_dir($path)) {
    drush_op('mkdir', $path);
    drush_log(dt('Directory @path was created', array('@path' => $path)), 'notice');
  }

  // Set the directory to the download location.
  $olddir = getcwd();
  chdir($path);

  $filename = basename(FEDANALYTICS_DOWNLOAD_URI);
  $dirname = basename('DAP-Gov-wide-GA-Code-master', '.zip');

  // Remove any existing fed_analytics directory.
  if (is_dir($dirname)) {
    drush_log(dt('A existing Colorbox plugin was overwritten at @path', array('@path' => $path)), 'notice');
  }
  // Remove any existing fed_analytics zip archive.
  if (is_file($filename)) {
    drush_op('unlink', $filename);
  }

  // Download the zip archive, use --no-check-certificate
  // flag to use wget with https.
  if (!drush_shell_exec('wget --no-check-certificate ' . FEDANALYTICS_DOWNLOAD_URI)) {
    drush_shell_exec('curl -O ' . FEDANALYTICS_DOWNLOAD_URI);
  }

  if (is_file($filename)) {
    // Decompress the zip archive.
    drush_shell_exec('unzip -qq -o ' . $filename);
    // Remove the zip archive.
    drush_op('unlink', $filename);
  }
  // Change the directory name to "fed_analytics" if needed.
  if ($dirname != 'fed_analytics') {
    drush_move_dir($dirname, 'fed_analytics', TRUE);
    $dirname = 'fed_analytics';
  }
  if (is_dir($dirname)) {
    drush_log(dt('The Federal Government Google Analytics library has been installed in @path', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to install the Federal Government Google Analytics library to @path', array('@path' => $path)), 'error');
  }
  // Set working directory back to the previous working directory.
  chdir($olddir);
}

