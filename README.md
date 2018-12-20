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
  * Terms: `/api/{vocabulary}/term/{id}`
  * Views: `/api/views/{view_name}`
  * Paragraphs: `/api/paragraphs/{type}/{id}`

License
-------

This project is GPL v2 software. See the LICENSE.txt file in this directory for
complete text.

Maintainers
-----------

- Geoff St. Pierre (https://github.com/serundeputy/)
