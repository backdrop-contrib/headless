<?php
/**
 * @file
 * Administrative page callbacks for the headless module.
 */

/**
 * Headless configuration form.
 */
function headless_settings_form($form, &$form_state) {
  $form['help'] = array(
    '#markup' => '<div class="headless-settings__help">' . t('Use this form to select wich entities you would like to expose as JSON endpoints.') . '</div>',
  );

  // Get all available content types.
  $nodeTypes = node_type_get_types();

  $form['nodes'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content types'),
    '#collapsible' => TRUE,
  );

  if (empty($nodeTypes)) {
    $form['nodes']['node-empty'] = array(
      '#markup' => t('No content types defined yet. <a href="@url">Manage content types</a>', array('@url' => url('admin/structure/types'))),
    );
  }
  else {
    $nodeTypeOptions = [];
    foreach($nodeTypes as $key => $type) {
      $nodeTypeOptions[$key] = $type->name;
    }
    $form['nodes']['node-checkboxes'] = array(
      '#type' => 'checkboxes',
      '#options' => $nodeTypeOptions,
      '#default_value' => _headless_get_default_options('node'),
    );
  }

  // Get all available vocabularies (if the Taxonomy module is enabled).
  if (module_exists('taxonomy')) {
    $vocabularyTypes = taxonomy_get_vocabularies();

    $form['vocabularies'] = array(
      '#type' => 'fieldset',
      '#title' => t('Taxonomy vocabularies'),
      '#collapsible' => TRUE,
      // Collapsed by default. Expand if no vocabularies defined.
      '#collapsed' => !empty($vocabularyTypes),
    );

    if (empty($vocabularyTypes)) {
      $form['vocabularies']['vocabularies-empty'] = array(
        '#markup' => t('No taxonomy vocabularies defined yet. <a href="@url">Manage vocabularies</a>', array('@url' => url('admin/structure/taxonomy'))),
      );
    }
    else {
      $vocabularyTypeOptions = [];
      foreach($vocabularyTypes as $key => $type) {
        $vocabularyTypeOptions[$key] = $type->name;
      }
      $form['vocabularies']['vocabularies-checkboxes'] = array(
        '#type' => 'checkboxes',
        '#options' => $vocabularyTypeOptions,
        '#default_value' => _headless_get_default_options('vocabularies'),
      );
    }

  }

  // Get all available views.
  $myViews = views_get_views_as_options(TRUE, 'enabled', NULL, TRUE, TRUE);

  $form['views'] = array(
    '#type' => 'fieldset',
    '#title' => t('Views'),
    '#collapsible' => TRUE,
    // Collapsed by default. Expand if no views defined.
    '#collapsed' => !empty($myViews),
  );

  if (empty($myViews)) {
    $no_views = t('No views defined yet.');
    if (module_exists('views_ui')) {
      $no_views .= ' ' . l(t('Manage views'), 'admin/structure/views');
    }
    $form['views']['views-empty'] = array(
      '#markup' => $no_views,
    );
  }
  else {
    $form['views']['views-checkboxes'] = array(
      '#type' => 'checkboxes',
      '#options' => $myViews,
      '#default_value' => _headless_get_default_options('views'),
    );
  }

  // Get all available paragraph types (if the Paragraphs module is enabled).
  if (module_exists('paragraphs')) {
    $paragraphsTypes = paragraphs_bundle_load();
    $paragraphsTypeOptions = [];

    $form['paragraphs'] = array(
      '#type' => 'fieldset',
      '#title' => t('Paragraph types'),
      '#collapsible' => TRUE,
      // Collapsed by default. Expand if no paragraph types defined.
      '#collapsed' => !empty($paragraphsTypes),
    );

    if (empty($paragraphsTypes)) {
      $form['paragraphs']['paragraphs-empty'] = array(
        '#markup' => t('No paragraphs defined yet. <a href="@url">Manage paragraph types</a>', array('@url' => url('admin/structure/paragraphs'))),
      );
    }
    else {
      foreach($paragraphsTypes as $key => $type) {
        $paragraphsTypeOptions[$key] = $key;
      }
      $form['paragraphs']['paragraphs-checkboxes'] = array(
        '#type' => 'checkboxes',
        '#options' => $paragraphsTypeOptions,
        '#default_value' => _headless_get_default_options('paragraphs'),
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
  foreach($form_state['input']['node-checkboxes'] as $type => $value) {
    if ($value != NULL) {
      $val = 1;
    }
    else {
      $val = 0;
    }
    config_set('headless.settings', 'node.' . $type, $val);
  }
  foreach($form_state['input']['vocabularies-checkboxes'] as $type => $value) {
    if ($value != NULL) {
      $val = 1;
    }
    else {
      $val = 0;
    }
    config_set('headless.settings', 'vocabularies.' . $type, $val);
  }
  foreach($form_state['input']['views-checkboxes'] as $type => $value) {
    if ($value != NULL) {
      $val = 1;
    }
    else {
      $val = 0;
    }
    config_set('headless.settings', 'views.' . $type, $val);
  }
  if (isset($form_state['input']['views-checkboxes'])) {
    foreach($form_state['input']['views-checkboxes'] as $type => $value) {
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
    foreach($form_state['input']['paragraphs-checkboxes'] as $type => $value) {
      if ($value != NULL) {
        $val = 1;
      }
      else {
        $val = 0;
      }
      config_set('headless.settings', 'paragraphs.' . $type, $val);
    }
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
  foreach(config_get('headless.settings', $types) as $key => $type) {
    if ($type != 0) {
      $checked[] = $key;
    }
  }

  return $checked;
}
