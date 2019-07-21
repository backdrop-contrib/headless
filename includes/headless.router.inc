<?php
/**
 * @file
 * Router: allow API queries via strings in URL path.
 * The router will try to resolve a string to an API asset.
 */

/**
 * Resolve a string path to an API asset.
 *
 * @param string $path
 *   The string to try to resolve to an API asset (usually a path).
 */
function headless_router(string $path) {
  $path = filter_xss($path);
  $assets = [];
  $query = db_select('url_alias', 'u')
    ->fields('u', ['source'])
    ->condition('alias', '%' . db_like($path) . '%', 'LIKE')
    ->addTag('node_access')
    ->execute();
  foreach ($query as $i => $asset) {
    $assets[] = $asset;
  }
  $source = $assets[0]->source;
  $source = explode('/', $source);
  if ($source[0] == 'node') {
    $node = node_load($source[1]);
    $type = $node->type;
    headless_type($type, $source[1]);
  }
  elseif ($source[0] == 'taxonomy') {
    $term = taxonomy_term_load($source[2]);
    headless_term_item($term->vocabulary, $source[2]);
  }
  else {
    $error = ['code' => 404];
    backdrop_json_output($error);
    backdrop_exit();
  }
}
