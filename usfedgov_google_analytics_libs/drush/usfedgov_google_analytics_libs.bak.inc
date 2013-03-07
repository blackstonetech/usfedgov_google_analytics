<?php

/**
 * @file
 *   drush integration for Federal Gocernment Google Analytics Library
 */

/**
 * The Federal Government Google Analytics js URI.
 */
define('FEDANALYTICS_DOWNLOAD_URI', 'https://github.com/GSA/DAP-Gov-wide-GA-Code/archive/master.zip');

/**
 * Implementation of hook_drush_command().
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
function usfedgov_google_anlytics_libs_drush_command() {
  $items = array();

  // the key in the $items array is the name of the command.
  $items['download_fed_analytics_libs'] = array(
    'callback' => 'drush_usfedgov_google_analytics_libs',
    'description' => dt("Downloads the federal google analytics plugins."),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH, // No bootstrap.
    'arguments' => array(
      'path' => dt('Optional. A path where to install the Colorbox plugin. If omitted Drush will use the default location.'),
    ),
    'aliases' => array('downloadfedanalyticslibs'),
  );

  return $items;
}

/**
 * Implementation of hook_drush_help().
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
      return dt("Downloads the Federal Government Google Analytics js files from github, default location is sites/all/libraries.");
  }
}


/**
 * Command to download the Colorbox plugin.
 */
function drush_usfedgov_google_analytics_libs() {
  if (drush_shell_exec('type unzip')) {
    return drush_set_error(dt('Missing dependency: unzip. Install it before using this command.'));
  }

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
  $dirname = basename(FEDANALYTICS_DOWNLOAD_URI, '.zip');

  // Remove any existing Colorbox plugin directory
  if (is_dir($dirname)) {
    drush_log(dt('A existing Colorbox plugin was overwritten at @path', array('@path' => $path)), 'notice');
  }
  // Remove any existing Colorbox plugin zip archive
  if (is_file($filename)) {
    drush_op('unlink', $filename);
  }

  // Download the zip archive, issue --no-check-certificate flag to use wget with https
  if (!drush_shell_exec('wget --no-check-certificate ' . FEDANALYTICS_DOWNLOAD_URI)) {
    drush_shell_exec('curl -O ' . FEDANALYTICS_DOWNLOAD_URI);
  }

  if (is_file($filename)) {
    // Decompress the zip archive
    drush_shell_exec('unzip -qq -o ' . $filename);
    // Remove the zip archive
    drush_op('unlink', $filename);
  }

  // Set working directory back to the previous working directory.
  chdir($olddir);

  if (is_dir($path . '/' . $dirname)) {
    drush_log(dt('Colorbox plugin has been downloaded to @path', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to download the Colorbox plugin to @path', array('@path' => $path)), 'error');
  }
}
