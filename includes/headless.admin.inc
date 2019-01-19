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
    '#markup' => '<div class="headless-settings__help">
      Use this form to select wich entities you would like to expose
      as json endpoints.
    </div>',
  );
  $form['nodes'] = array(
    '#type' => 'fieldset',
    '#title' => 'Node Types',
    '#collapsible' => TRUE,
  );
  // Get the node types.
  $nodeTypes = node_type_get_types();
  $nodeTypeOptions = [];
  foreach($nodeTypes as $key => $type) {
    $nodeTypeOptions[$key] = $key;
  }
  $form['nodes']['node-checkboxes'] = array(
    '#type' => 'checkboxes',
    '#options' => $nodeTypeOptions,
    '#default_value' => _headless_get_default_options('node'),
  );
  $vocabularyTypes = taxonomy_get_vocabularies();
  $vocabularyTypeOptions = [];
  foreach($vocabularyTypes as $key => $type) {
    $vocabularyTypeOptions[$key] = $key;
  }
  $form['vocabularies'] = array(
    '#type' => 'fieldset',
    '#title' => 'Vocabulary Types',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['vocabularies']['vocabularies-checkboxes'] = array(
    '#type' => 'checkboxes',
    '#options' => $vocabularyTypeOptions,
    '#default_value' => _headless_get_default_options('vocabularies'),
  );
  $form['views'] = array(
    '#type' => 'fieldset',
    '#title' => 'Views',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $myViews = views_get_views_as_options(TRUE, 'enabled', NULL, TRUE, TRUE);
  $form['views']['views-checkboxes'] = array(
    '#type' => 'checkboxes',
    '#options' => $myViews,
    '#default_value' => _headless_get_default_options('views'),
  );
  if (module_exists('paragraphs')) {
    $paragraphsTypes = paragraphs_bundle_load();
    $paragraphsTypeOptions = [];
    foreach($paragraphsTypes as $key => $type) {
      $paragraphsTypeOptions[$key] = $key;
    }
    $form['paragraphs'] = array(
      '#type' => 'fieldset',
      '#title' => 'Paragraphs Types',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['paragraphs']['paragraphs-checkboxes'] = array(
      '#type' => 'checkboxes',
      '#options' => $paragraphsTypeOptions,
      '#default_value' => _headless_get_default_options('paragraphs'),
    );
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
