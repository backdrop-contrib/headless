<?php

/**
 * @file
 * V2 functions for Headless module.
 */


/**
 * Page callback for node types v2.
 */
function headless_type_v2($type, $nid)
{
  // Check if the JSON output is enabled for the selected type.
  $config = config_get('headless.settings', 'node');
  if ($config[$type] != 1) {
    $json_error = ['code' => 404];
    backdrop_json_output($json_error);
    backdrop_exit();
  }
  // Load the requested node and check to see if types match or not.
  $requested_node = node_load($nid);
  if ($requested_node->type == $type) {
    $my_json = node_view($requested_node);
    backdrop_json_output($my_json);
    backdrop_exit();
  } else {
    $json_error = ['code' => 404];
    backdrop_json_output($json_error);
    backdrop_exit();
  }
}

/**
 * Page callback for views v2.
 */
function headless_views_v2($view, $display_id)
{
  $args = func_get_args();
  $config = config_get('headless.settings', 'views');
  if ($config[$view] != 1) {
    $json_error = ['code' => 404];
    backdrop_json_output($json_error);
    backdrop_exit();
  }
  $return = _views_get_view_result($view, $display_id, $args);

  $my_json = [
    'results' => $return['results'],
    'total_items' => $return['total_items'],
    'items_per_page' => $return['items_per_page'],
    'total_pages' => (isset($return['items_per_page'])) ? ceil($return['total_items'] / $return['items_per_page']) : null,
    'current_page' => $return['current_page'],
  ];
  backdrop_json_output($my_json);
  json_last_error_msg();
  backdrop_exit();
}

/**
 * Page callback for paragraph types v2.
 */
function headless_paragraphs_item_v2($type, $entity_id)
{
  $config = config_get('headless.settings', 'paragraphs');
  if ($config[$type] != 1) {
    $json_error = ['code' => 404];
    backdrop_json_output($json_error);
    backdrop_exit();
  }
  $result = paragraphs_item_load($entity_id)->view();
  backdrop_json_output($result);
  backdrop_exit();
}
