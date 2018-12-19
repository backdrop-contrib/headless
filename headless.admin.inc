<?php
/**
 * @file
 * Administrative page callbacks for the headless module.
 */

/**
 * Headless configuration form.
 */
function headless_settings_form($form, &$form_state) {
  // Get the node types.
  $nodeTypes = node_type_get_types();
  $nodeTypeOptions = [];
  foreach($nodeTypes as $key => $type) {
    $nodeTypeOptions[$key] = $key;
  }
  $form['help'] = array(
    '#markup' => '<div class="headless-settings__help">
      Use this form to select wich entities you would like to expose
      as json endpoints.
    </div>',
  );
  $form['nodes'] = array(
    '#type' => 'fieldset',
    '#title' => 'Node Types',
  );
  $form['nodes']['node-checkboxes'] = array(
    '#type' => 'checkboxes',
    '#options' => $nodeTypeOptions,
    '#title' =>
      t('Check off the Node Types you would like to expose as json endpoints.'),
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
    );
    $form['paragraphs']['paragraphs-checkboxes'] = array(
      '#type' => 'checkboxes',
      '#options' => $paragraphsTypeOptions,
      '#title' =>
        t('Check off the Paragraph Types you would like to expose as json endpoints.'),
    );
  }
  $vocabularyTypes = taxonomy_get_vocabularies();
  $vocabularyTypeOptions = [];
  foreach($vocabularyTypes as $key => $type) {
    $vocabularyTypeOptions[$key] = $key;
  }
  $form['vocabularies'] = array(
    '#type' => 'fieldset',
    '#title' => 'Vocabulary Types',
  );
  $form['vocabularies']['vocabularies-checkboxes'] = array(
    '#type' => 'checkboxes',
    '#options' => $vocabularyTypeOptions,
    '#title' =>
      t('Check off the Paragraph Types you would like to expose as json endpoints.'),
  );

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

  path_save($form_state['values']);

  backdrop_set_message(t('The alias has been saved.'));
  $form_state['redirect'] = 'admin/config/urls/path';
}
