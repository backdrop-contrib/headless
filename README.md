Headless
========

Headless is a read only API to deliver Backdrop nodes, terms, views, and
paragraphs as json endpoints.

Features
--------

This allows you to publish content to your Backdrop site and send it out to
other applications.  For example you could have a [nuxt](https://nuxtjs.org)
front end app and an Apache Cordovo app both pulling in content from the
endpoints.

In this way you get all the power of Backdrop CMS:
  - Authoring experience
  - Structured content
    - Custom content types
    - fieldable
    - views

That you can deliver to any consumer app you want.


Usage
-----

To configure which entities you would like to expose as json endpoints:

  * Visit: `/admin/config/services/headless`
    * Check off the entities you would like to expose
    * Save the configuration form

Endpoints:

  * Nodes: `/api/node/{type}/{id}`
    * for example the default `/about` page is available at `/api/node/page/2`
      * Assuming you've checked off to allow the `page` type as exposed on
        `/admin/config/services/headless`
  * Nodes (v2): `/api/v2/node/{type}/{id}`
    * It uses the display configuration for node types, which gives more flexibility as well as some data that was missing with the original end point, like term names on term reference fields.
  * Terms: `/api/{vocabulary}/term/{id}`
  * Views: `/api/views/{view_name}`
    * Kept for backwards compatibility.
  * Views (v2): `/api/v2/views/{view_name}/{display_id}/{contextual_arguments}{filter_arguments}`
    * `view_name` and `display_id` are required.
    * `contextual_arguments` are NOT required. They can be any contextual arguments you have set in the view, separated by a `/`.
    * `filter_arguments` are NOT required and can be:
      * ?page={x}
        * to show only results for a given page. Pages start at 0
        * example: `?page=0`
      * ?{exposed_filter}={value}
        * you can use also any exposed filter you have set for your view
        * example: show only results with a title which contains a string use `?title=some_string`
      * You can combine filter arguments by chaning them wiht a `&`
        * example: `?type=post&title=string`
     * The best way to see what are the arguments you can use is by creating the exposed filters and/or contextual arguments, then go to the view, do a search and look at the url that it generates.
  * Paragraphs: `/api/paragraphs/{type}/{id}`
    * It shows load data for a given paragraphs item
  * Paragraphs (v2): `/api/v2/paragraphs/{type}/{id}`
    * It shows display data for a given paragraphs item
  * Custom blocks: `/api/blocks/{block_name}`

License
-------

This project is GPL v2 software. See the LICENSE.txt file in this directory for
complete text.

Maintainers
-----------

- Geoff St. Pierre (https://github.com/serundeputy/)
- Robert Garrigos (https://github.com/robertgarrigos)
