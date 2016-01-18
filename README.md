Headless
---
Headless is an API to deliver nodes and lists of nodes as JSON data.

This allows you to publish content to you Backdrop site and send it out to other applications.  For example you could have an
Apache Cordovo app that polls for the latest news posts on your site and renders them in a native iOS or Android app on users'
mobile phones.

* Exposes individual nodes as JSON data to be consumed by another application.
  * Once the `headless` module is installed visiting a URL using this form: http://mysite.com/api/page/16 will expose that node as a JSON object.
    * You can replace `page` with any content type on your site
    * You replace `16` with the node id (`nid`) of the content that you want to recieve as JSON data. 
* Expose lists of nodes as JSON data.
  * Visit a URL using this form:  http://mysite.com/api/list/post

License
-------

This project is GPL v2 software. See the LICENSE.txt file in this directory for
complete text.

Maintainers
-----------

- Geoff St. Pierre (https://github.com/serundeputy/)
