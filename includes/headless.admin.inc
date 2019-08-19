<?php

/**
 * @file
 * Administrative page callbacks for the headless module.
 */

/**
 * Headless configuration form.
 */
function headless_settings_form($form, &$form_state) {
  $help = '
    <div class="headless-settings__help">
      Use this form to select wich entities you would like to expose as json endpoints.
    </div>
  ';
  $form['help'] = array(
    '#markup' => $help,
  );

  // Get all available content types.
  $node_types = node_type_get_types();

  $form['nodes'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content types'),
    '#collapsible' => TRUE,
  );

  if (empty($node_types)) {
    $form['nodes']['node-empty'] = array(
      '#markup' => t('No content types defined yet. <a href="@url">Manage content types</a>', array('@url' => url('admin/structure/types'))),
    );
  }
  else {
    $node_type_header = array(
      'type' => t('Type'),
      'endpoint_v1' => t('End point v1'),
      'endpoint_v2' => t('End point v2'),
    );
    $node_type_options = [];
    foreach ($node_types as $key => $type) {
      $node_type_options[$key] = array(
        'type' => $type->name,
        'endpoint_v1' => array(
          'data' => array(
            '#type' => 'link',
            '#title' => '/api/node/' . $key . '/{id}',
            '#href' => '/api/node/' . $key . '/{id}',
          ),
        ),
        'endpoint_v2' => array(
          'data' => array(
            '#type' => 'link',
            '#title' => '/api/v2/node/' . $key . '/{id}',
            '#href' => '/api/v2/node/' . $key . '/{id}',
          ),
        ),
      );
    }
    $form['nodes']['node-checkboxes'] = array(
      '#type' => 'tableselect',
      '#header' => $node_type_header,
      '#options' => $node_type_options,
      '#default_value' => _headless_get_default_options('node'),
      '#js_select' => FALSE,
    );
  }

  // Get all available vocabularies (if the Taxonomy module is enabled).
  if (module_exists('taxonomy')) {
    $vocabulary_types = taxonomy_get_vocabularies();

    $form['vocabularies'] = array(
      '#type' => 'fieldset',
      '#title' => t('Taxonomy vocabularies'),
      '#collapsible' => TRUE,
      // Collapsed by default. Expand if no vocabularies defined.
      // '#collapsed' => !empty($vocabulary_types),
    );

    if (empty($vocabulary_types)) {
      $form['vocabularies']['vocabularies-empty'] = array(
        '#markup' => t('No taxonomy vocabularies defined yet. <a href="@url">Manage vocabularies</a>', array('@url' => url('admin/structure/taxonomy'))),
      );
    }
    else {
      $vocabulary_type_header = array(
        'vocabulary' => t('Vocabulary'),
        'endpoint' => t('End point'),
      );
      $vocabulary_type_options = [];
      foreach ($vocabulary_types as $key => $type) {
        $vocabulary_type_options[$key] = array(
          'vocabulary' => $type->name,
          'endpoint' => array(
            'data' => array(
              '#type' => 'link',
              '#title' => '/api/' . $key . '/term/{id}',
              '#href' => '/api/' . $key . '/term/{id}',
            ),
          ),
        );
      }
      $form['vocabularies']['vocabularies-checkboxes'] = array(
        '#type' => 'tableselect',
        '#header' => $vocabulary_type_header,
        '#options' => $vocabulary_type_options,
        '#default_value' => _headless_get_default_options('vocabularies'),
        '#js_select' => FALSE,
      );
    }
  }

  // Get all available views.
  $my_views = views_get_views_as_options(TRUE, 'enabled', NULL, TRUE, TRUE);

  $form['views'] = array(
    '#type' => 'fieldset',
    '#title' => t('Views'),
    '#collapsible' => TRUE,
    // Collapsed by default. Expand if no views defined.
    // '#collapsed' => !empty($my_views),
  );

  if (empty($my_views)) {
    $no_views = t('No views defined yet.');
    if (module_exists('views_ui')) {
      $no_views .= ' ' . l(t('Manage views'), 'admin/structure/views');
    }
    $form['views']['views-empty'] = array(
      '#markup' => $no_views,
    );
  }
  else {
    $my_views_header = array(
      'view' => t('View'),
      'endpoint_v1' => t('End point v1'),
      'endpoint_v2' => t('End point v2'),
    );
    foreach ($my_views as $key => $view) {
      $my_views_options[$key] = array(
        'view' => $view,
        'endpoint_v1' => array(
          'data' => array(
            '#type' => 'link',
            '#title' => '/api/views/' . $key,
            '#href' => '/api/views/' . $key,
          ),
        ),
        'endpoint_v2' => array(
          'data' => array(
            '#type' => 'link',
            '#title' => '/api/v2/views/' . $key . '/{display_id}{arguments}',
            '#href' => '/api/v2/views/' . $key . '/{display_id}{arguments}',
          ),
        ),
      );
    }
    $form['views']['views-checkboxes'] = array(
      '#type' => 'tableselect',
      '#header' => $my_views_header,
      '#options' => $my_views_options,
      '#default_value' => _headless_get_default_options('views'),
      '#js_select' => FALSE,
    );
  }

  // Get all available paragraph types (if the Paragraphs module is enabled).
  if (module_exists('paragraphs')) {
    $paragraphs_types = paragraphs_bundle_load();
    $paragraphs_type_options = [];

    $form['paragraphs'] = array(
      '#type' => 'fieldset',
      '#title' => t('Paragraph types'),
      '#collapsible' => TRUE,
      // Collapsed by default. Expand if no paragraph types defined.
      // '#collapsed' => !empty($paragraphs_types),
    );

    if (empty($paragraphs_types)) {
      $form['paragraphs']['paragraphs-empty'] = array(
        '#markup' => t('No paragraphs defined yet. <a href="@url">Manage paragraph types</a>', array('@url' => url('admin/structure/paragraphs'))),
      );
    }
    else {
      $paragraphs_type_header = array(
        'paragraph' => t('paragraph'),
        'endpoint' => t('End point'),
      );
      foreach ($paragraphs_types as $key => $type) {
        $paragraphs_type_options[$key] = array(
          'paragraph' => $type->name,
          'endpoint' => array(
            'data' => array(
              '#type' => 'link',
              '#title' => '/api/paragraphs/' . $key . '/{id}',
              '#href' => '/api/paragraphs/' . $key . '/{id}',
            ),
          ),
        );
      }
      $form['paragraphs']['paragraphs-checkboxes'] = array(
        '#type' => 'tableselect',
        '#header' => $paragraphs_type_header,
        '#options' => $paragraphs_type_options,
        '#default_value' => _headless_get_default_options('paragraphs'),
        '#js_select' => FALSE,
      );
    }
  }

  // Get all available custom blocks.
  $my_blocks = block_block_info();

  $form['blocks'] = array(
    '#type' => 'fieldset',
    '#title' => t('Custom blocks'),
    '#collapsible' => TRUE,
  );

  if (empty($my_blocks)) {
    $no_blocks = t('No blocks defined yet.');
    if (module_exists('block')) {
      $no_blocks .= ' ' . l(t('Manage custom blocks'), 'admin/structure/block');
    }
    $form['blocks']['blocks-empty'] = array(
      '#markup' => $no_blocks,
    );
  }
  else {
    $my_blocks_header = array(
      'block' => t('Block'),
      'endpoint_v1' => t('End point'),
    );
    foreach ($my_blocks as $key => $block) {
      $my_blocks_options[$key] = array(
        'block' => $key,
        'endpoint_v1' => array(
          'data' => array(
            '#type' => 'link',
            '#title' => '/api/blocks/' . $key,
            '#href' => '/api/blocks/' . $key,
          ),
        ),
      );
    }
    $form['blocks']['blocks-checkboxes'] = array(
      '#type' => 'tableselect',
      '#header' => $my_blocks_header,
      '#options' => $my_blocks_options,
      '#default_value' => _headless_get_default_options('blocks'),
      '#js_select' => FALSE,
    );

    if (empty($paragraphs_types)) {
      $form['paragraphs']['paragraphs-empty'] = array(
        '#markup' => t('No paragraphs defined yet. <a href="@url">Manage paragraph types</a>', array('@url' => url('admin/structure/paragraphs'))),
      );
    }
    else {
      $paragraphs_type_header = array(
        'paragraph' => t('paragraph'),
        'endpoint' => t('End point'),
      );
      foreach ($paragraphs_types as $key => $type) {
        $paragraphs_type_options[$key] = array(
          'paragraph' => $type->name,
          'endpoint' => array(
            'data' => array(
              '#type' => 'link',
              '#title' => '/api/paragraphs/' . $key . '/{id}',
              '#href' => '/api/paragraphs/' . $key . '/{id}',
            ),
          ),
        );
      }
      $form['paragraphs']['paragraphs-checkboxes'] = array(
        '#type' => 'tableselect',
        '#header' => $paragraphs_type_header,
        '#options' => $paragraphs_type_options,
        '#default_value' => _headless_get_default_options('paragraphs'),
        '#js_select' => FALSE,
      );
    }
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Configuration'),
  );

  return $form;
}

/**
 * Form submission handler for headless_settings_form().
 */
function headless_settings_form_submit($form, &$form_state) {
  // Remove unnecessary values.
  form_state_values_clean($form_state);
  foreach ($form_state['input']['node-checkboxes'] as $type => $value) {
    if ($value != NULL) {
      $val = 1;
    }
    else {
      $val = 0;
    }
    config_set('headless.settings', 'node.' . $type, $val);
  }
  foreach ($form_state['input']['vocabularies-checkboxes'] as $type => $value) {
    if ($value != NULL) {
      $val = 1;
    }
    else {
      $val = 0;
    }
    config_set('headless.settings', 'vocabularies.' . $type, $val);
  }
  foreach ($form_state['input']['views-checkboxes'] as $type => $value) {
    if ($value != NULL) {
      $val = 1;
    }
    else {
      $val = 0;
    }
    config_set('headless.settings', 'views.' . $type, $val);
  }
  if (isset($form_state['input']['views-checkboxes'])) {
    foreach ($form_state['input']['views-checkboxes'] as $type => $value) {
      if ($value != NULL) {
        $val = 1;
      }
      else {
        $val = 0;
      }
      config_set('headless.settings', 'views.' . $type, $val);
    }
  }
  if (isset($form_state['input']['paragraphs-checkboxes'])) {
    foreach ($form_state['input']['paragraphs-checkboxes'] as $type => $value) {
      if ($value != NULL) {
        $val = 1;
      }
      else {
        $val = 0;
      }
      config_set('headless.settings', 'paragraphs.' . $type, $val);
    }
  }
  foreach ($form_state['input']['blocks-checkboxes'] as $type => $value) {
    if ($value != NULL) {
      $val = 1;
    }
    else {
      $val = 0;
    }
    config_set('headless.settings', 'blocks.' . $type, $val);
  }

  backdrop_set_message(t('The Headless configuration has been saved.'));
  $form_state['redirect'] = 'admin/config/services/headless';
}

/**
 * Helper function to get the #default_options for a type.
 *
 * @param string $types
 *   The type of entity to get the defaults for one of:
 *     - node
 *     - vocabularies
 *     - views
 *     - paragraphs
 *
 * @return array
 *   An array of the currently selected types.
 */
function _headless_get_default_options($types) {
  $checked = [];
  foreach (config_get('headless.settings', $types) as $key => $type) {
    if ($type != 0) {
      $checked[$key] = TRUE;
    }
  }

  return $checked;
}
