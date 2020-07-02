<?php

/**
 * @file
 * V3 functions for Headless module.
 */


/**
 * Page callback for paragraph types v3.
 */
function headless_paragraphs_item_v3($entity_id) {
  $config = config_get('headless.settings', 'paragraphs');
  $p = paragraphs_item_load($entity_id);
  if ($config[$p->bundle] != 1) {
    $json_error = ['code' => 404];
    backdrop_json_output($json_error);
    backdrop_exit();
  }
  $result = $p->view();
  backdrop_json_output($result);
  backdrop_exit();
}
