 var _ = require('/usr/lib/node_modules/lodash')
 var $ = require('/usr/lib/node_modules/jquery')

/* vim: set expandtab tabstop=2 shiftwidth=2 softtabstop=2 cc=76; */

/**
 * SVG-to-WKT.
 *
 * @package     svg-to-wkt
 * @copyright   2012 David McClure
 * @license     http://www.apache.org/licenses/LICENSE-2.0.html
 */

(function() {


  var SVGNS = 'http://www.w3.org/2000/svg';
  var SVGtoWKT = {};


  /**
   * The number of decimal places computed during curve interpolation when
   * generating points for `<circle>`, `<ellipse>`, and `<path>` elements.
   *
   * @public
   */
  SVGtoWKT.PRECISION = 3;


  /**
   * The number of points computed during curve interpolation per unit of
   * linear pixel length. For example, if a a path is 10px in length, and
   * `DENSITY` is set to 2, the path will be rendered with 20 points.
   *
   * @public
   */
  SVGtoWKT.DENSITY = 1;


  /**
   * SVG => WKT.
   *
   * @param {String} svg: SVG markup.
   * @return {String}: Generated WKT.
   *
   * @public
   */
  SVGtoWKT.convert = function(svg) {

    // Halt if svg is undefined or empty.
    if (_.isUndefined(svg) || _.isEmpty(svg.trim())) {
      throw new Error('Empty XML.');
    }

    var els = [];
    var xml;

    // Strip out tabs and linebreaks.
    svg = svg.replace(/\r\n|\r|\n|\t/g, '');
    console.log($.parseXML)
    try {
      // Parse the raw XML.
      let parse = $.parseXML(svg)
      xml = $(parse);
    } catch (e) {
      // Halt if malformed.
      throw new Error('Invalid XMl.');
    }

    // Match `<polygon>` elements.
    xml.find('polygon').each(function(i, polygon) {
      els.push(SVGtoWKT.polygon($(polygon).attr('points')));
    });

    // Match `<polyline>` elements.
    xml.find('polyline').each(function(i, polyline) {
      els.push(SVGtoWKT.polyline($(polyline).attr('points')));
    });

    // Match `<line>` elements.
    xml.find('line').each(function(i, line) {
      els.push(SVGtoWKT.line(
        parseFloat($(line).attr('x1')),
        parseFloat($(line).attr('y1')),
        parseFloat($(line).attr('x2')),
        parseFloat($(line).attr('y2'))
      ));
    });

    // Match `<rect>` elements.
    xml.find('rect').each(function(i, rect) {
      els.push(SVGtoWKT.rect(
        parseFloat($(rect).attr('x')),
        parseFloat($(rect).attr('y')),
        parseFloat($(rect).attr('width')),
        parseFloat($(rect).attr('height'))
      ));
    });

    // Match `<circle>` elements.
    xml.find('circle').each(function(i, circle) {
      els.push(SVGtoWKT.circle(
        parseFloat($(circle).attr('cx')),
        parseFloat($(circle).attr('cy')),
        parseFloat($(circle).attr('r'))
      ));
    });

    // Match `<ellipse>` elements.
    xml.find('ellipse').each(function(i, circle) {
      els.push(SVGtoWKT.ellipse(
        parseFloat($(circle).attr('cx')),
        parseFloat($(circle).attr('cy')),
        parseFloat($(circle).attr('rx')),
        parseFloat($(circle).attr('ry'))
      ));
    });

    // Match `<path>` elements.
    xml.find('path').each(function(i, path) {
      els.push(SVGtoWKT.path($(path).attr('d')));
    });

    return 'GEOMETRYCOLLECTION(' + els.join(',') + ')';

  };


  /**
   * Construct a WKT line from SVG start/end point coordinates.
   *
   * @param {Number} x1: Start X.
   * @param {Number} y1: Start Y.
   * @param {Number} x2: End X.
   * @param {Number} y2: End Y.
   * @return {String}: Generated WKT.
   *
   * @public
   */
  SVGtoWKT.line = function(x1, y1, x2, y2) {
    return 'LINESTRING('+x1+' '+-y1+','+x2+' '+-y2+')';
  };


  /**
   * Construct a WKT linestrimg from SVG `points` attribute value.
   *
   * @param {String} points: <polyline> `points` attribute value.
   * @return {String}: Generated WKT.
   *
   * @public
   */
  SVGtoWKT.polyline = function(points) {

    // "1,2 3,4 " => "1 2,3 4"
    var pts = _.map(points.trim().split(' '), function(pt) {
      pt = pt.split(','); pt[1] = -pt[1];
      return pt.join(' ');
    });

    return 'LINESTRING(' + pts.join() + ')';

  };


  /**
   * Construct a WKT polygon from SVG `points` attribute value.
   *
   * @param {String} points: <polygon> `points` attribute value.
   * @return {String}: Generated WKT.
   *
   * @public
   */
  SVGtoWKT.polygon = function(points) {

    // "1,2 3,4 " => "1 2,3 4"
    var pts = _.map(points.trim().split(' '), function(pt) {
      pt = pt.split(','); pt[1] = -pt[1];
      return pt.join(' ');
    });

    // Close.
    pts.push(pts[0]);

    return 'POLYGON((' + pts.join() + '))';

  };


  /**
   * Construct a WKT polygon from SVG rectangle origin and dimensions.
   *
   * @param {Number} x: Top left X.
   * @param {Number} y: Top left Y.
   * @param {Number} width: Rectangle width.
   * @param {Number} height: Rectangle height.
   * @return {String}: Generated WKT.
   *
   * @public
   */
  SVGtoWKT.rect = function(x, y, width, height) {

    var pts = [];

    // 0,0 origin by default.
    if (!_.isNumber(x)) x = 0;
    if (!_.isNumber(y)) y = 0;

    // No corner rounding.
    pts.push(String(x)+' '+String(-y));              // top left
    pts.push(String(x+width)+' '+String(-y));        // top right
    pts.push(String(x+width)+' '+String(-y-height)); // bottom right
    pts.push(String(x)+' '+String(-y-height));       // bottom left
    pts.push(String(x)+' '+String(-y));              // close

    // TODO|dev: Corner rounding.

    return 'POLYGON((' + pts.join() + '))';

  };


  /**
   * Construct a WKT polygon for a circle from origin and radius.
   *
   * @param {Number} cx: Center X.
   * @param {Number} cy: Center Y.
   * @param {Number} r: Radius.
   * @return {String} wkt: Generated WKT.
   *
   * @public
   */
  SVGtoWKT.circle = function(cx, cy, r) {

    var wkt = 'POLYGON((';
    var pts = [];

    // Compute number of points.
    var circumference = Math.PI * 2 * r;
    var point_count = Math.round(circumference * SVGtoWKT.DENSITY);

    // Compute angle between points.
    var interval_angle = 360 / point_count;

    // Genrate the circle.
    _(point_count).times(function(i) {
      var angle = (interval_angle * i) * (Math.PI / 180);
      var x = __round(cx + r * Math.cos(angle));
      var y = __round(cy + r * Math.sin(angle));
      pts.push(String(x)+' '+String(-y));
    });

    // Close.
    pts.push(pts[0]);

    return wkt + pts.join() + '))';

  };


  /**
   * Construct a WKT polygon for an ellipse from origin and radii.
   *
   * @param {Number} cx: Center X.
   * @param {Number} cy: Center Y.
   * @param {Number} rx: Horizontal radius.
   * @param {Number} ry: Vertical radius.
   * @return {String} wkt: Generated WKT.
   *
   * @public
   */
  SVGtoWKT.ellipse = function(cx, cy, rx, ry) {

    var wkt = 'POLYGON((';
    var pts = [];

    // Approximate the circumference.
    var circumference = 2 * Math.PI * Math.sqrt(
      (Math.pow(rx, 2) + Math.pow(ry, 2)) / 2
    );

    // Compute number of points and angle between points.
    var point_count = Math.round(circumference * SVGtoWKT.DENSITY);
    var interval_angle = 360 / point_count;

    // Generate the ellipse.
    _(point_count).times(function(i) {
      var angle = (interval_angle * i) * (Math.PI / 180);
      var x = __round(cx + rx * Math.cos(angle));
      var y = __round(cy + ry * Math.sin(angle));
      pts.push(String(x)+' '+String(-y));
    });

    // Close.
    pts.push(pts[0]);

    return wkt + pts.join() + '))';

  };


  /**
   * Construct a WKT polygon from a SVG path string. Approach from:
   * http://whaticode.com/2012/02/01/converting-svg-paths-to-polygons/
   *
   * @param {String} d: <path> `d` attribute value.
   * @return {String}: Generated WKT.
   *
   * @public
   */
  SVGtoWKT.path = function(d) {

    // Try to extract polygon paths closed with 'Z'.
    var polys = _.map(d.trim().match(/[^z|Z]+[z|Z]/g), function(p) {
      return __pathElement(p.trim()+'z');
    });

    // If closed polygon paths exist, construct a `POLYGON`.
    if (!_.isEmpty(polys)) {

      var parts = [];
      _.each(polys, function(poly) {
        parts.push('(' + __pathPoints(poly).join() + ')');
      });

      return 'POLYGON(' + parts.join() + ')';

    }

    // Otherwise, construct a `LINESTRING` from the unclosed path.
    else {
      var line = __pathElement(d);
      return 'LINESTRING(' + __pathPoints(line).join() + ')';
    }

  };


  /**
   * Construct a SVG path element.
   *
   * @param {String} d: <path> `d` attribute value.
   * @return {SVGPathElement}: The new <path> element.
   *
   * @private
   */
  var __pathElement = function(d) {
    var path = document.createElementNS(SVGNS, 'path');
    path.setAttributeNS(null, 'd', d);
    return path;
  };


  /**
   * Construct a SVG path element.
   *
   * @param {SVGPathElement} path: A <path> element.
   * @param {Boolean} closed: True if the path should be closed.
   * @return array: An array of space-delimited coords.
   *
   * @private
   */
  var __pathPoints = function(path, closed) {

    closed = closed || false;
    var pts = [];

    // Get number of points.
    var length = path.getTotalLength();
    var count = Math.round(length * SVGtoWKT.DENSITY);

    // Interpolate points.
    _(count+1).times(function(i) {
      var point = path.getPointAtLength((length * i) / count);
      pts.push(String(__round(point.x))+' '+String(__round(-point.y)));
    });

    if (closed) pts.push(pts[0]);
    return pts;

  };


  /**
   * Round a number to the number of decimal places in `PRECISION`.
   *
   * @param {Number} val: The number to round.
   * @return {Number}: The rounded value.
   *
   * @private
   */
  var __round = function(val) {
    var root = Math.pow(10, SVGtoWKT.PRECISION);
    return Math.round(val * root) / root;
  };


  this.SVGtoWKT = SVGtoWKT;


})();

/** End svg-to-wkt.js **/

document.getElementById('submit').onclick = function() {
  var svg = document.getElementById('svg').value;
  var wkt = SVGtoWKT.convert(svg);
  
  document.getElementById('output').innerText = wkt;
}

 let paths = [
   "<path class='st5' d='M20.8,31.1c0,0,1.4,1.4,4.9,2.1c3.5,0.7,7.4,1.3,10.7,0.9c2.6-0.3,5.2-1,7.6-2c0.4-0.3,0.9-0.5,1.4-0.6l0.2,1.1c0,0-4.2,3.6-12.6,3.5C24.7,36,20.1,32,20.1,32L20.8,31.1z'/>",
  "<path class='st5' d='M19.5,15.5c1.8,2,2.8,4.5,2.8,7.2c0.2,4.8,0.4,9.8,0.6,10.1c1.1,0.5,2.2,0.8,3.3,1.1c0.4,0.1,0.8-9.5,1-12c0-1.8-0.4-3.5-1.3-5.1c-0.4-1-1.8-3.4-3.4-3.3S19.8,13.7,19.5,15.5z'/>",
  "<path class='st5' d='M46.7,15.5c-1.8,2-2.8,4.5-2.8,7.2c-0.2,4.8-0.4,9.8-0.6,10.1c-1.1,0.5-2.2,0.8-3.3,1.1c-0.4,0.1-0.8-9.5-1-12c0-1.8,0.4-3.5,1.3-5.1c0.4-1,1.8-3.4,3.4-3.3C45.3,13.4,46.4,13.7,46.7,15.5z'/>",
  "<path class='st4' d='M41.6,17.1c0.3-0.9,1-1.6,1.8-2.1c0.6-0.3,1.2-0.4,1.9-0.4C43.9,15.3,42.7,16.1,41.6,17.1z'/>",
  "<path class='st4' d='M25,17.1c-0.3-0.9-1-1.6-1.8-2.1c-0.6-0.3-1.2-0.4-1.9-0.4C22.7,15.3,23.9,16.1,25,17.1z'/>",
  "<path class='st7' d='M45.9,14c-0.6-0.2-1.6-1.1-2.6-0.6c-1.1,0.5-0.9-0.5-1.9,1.4c-1,2-2.2,4-3.4,6c-0.6,0.8-1.3,1.5-2.2,2c-0.8,0.4,2.7-3.9,3.2-6.3c0.5-2.4,0.7-5.8-0.7-7.8c-1.4-2-1.9-3.3-5.2-3.8c-3.3-0.6-0.2-1,1.1-1c2.9,0.4,5.6,1.2,8.3,2.3c2.8,1.1,3.3,2.8,3.3,3C45.7,9.3,45.9,14,45.9,14z'/>",
  "<path class='st8' d='M33.9,4.5c3.4,0.2,6.7,1.2,9.7,2.9c5.1,2.9,5.4,12.8,5.4,14.2c0.1,1.4,0.5,1.4,0.5,3.3v8.6c0,0.1,0,0.1-0.1,0.2l-1.8,2.3c0,0-0.7,8-0.7,8.3s-6,10-6.8,11.1s-2.2,1.4-4,1.4H30c-1.8,0-3.2-0.4-4-1.4c-0.8-1.1-6.8-10.7-6.8-11.1s-0.7-8.3-0.7-8.3l-1.8-2.3c0,0-0.1-0.1-0.1-0.2v-8.5c0-1.9,0.5-1.9,0.5-3.3c0.1-1.4,0.3-11.3,5.4-14.2c3-1.7,6.3-2.7,9.7-2.9L33.9,4.5 M33.9,3.5h-1.6c-3.6,0.2-7,1.3-10.1,3c-4.8,2.7-5.8,10.5-5.9,14.8v0.1c0,0.4-0.1,0.7-0.2,1.1c-0.2,0.7-0.4,1.5-0.3,2.3v8.5c0,0.3,0.1,0.6,0.3,0.8l1.6,2.1c0.3,3.1,0.7,7.6,0.7,8c0,0.9,6.9,11.5,6.9,11.5c0.9,1.3,2.5,1.9,4.8,1.9h6.2c2.3,0,3.9-0.6,4.8-1.9c0.1-0.1,6.9-10.7,6.9-11.7c0-0.3,0.4-4.9,0.7-7.9l1.6-2.1c0.2-0.2,0.3-0.5,0.3-0.8v-8.4c0-0.8-0.1-1.6-0.3-2.3C50,22.3,50,21.9,50,21.6v-0.1C49.8,17,48.8,9.3,44,6.6C40.9,4.8,37.5,3.7,33.9,3.5L33.9,3.5z'/>",
  "<path class='st8' d='M33.4,50.4c1.7,0,3.4-0.2,5.1-0.4c0.1,0,1.4,1.2,1.4,1.2l1.8-2.4l0.3,0.5L40,52.3c0,0-1.4-1.2-1.4-1.2c-1.7,0.2-3.4,0.4-5.1,0.4H33c-1.7,0-3.4-0.2-5.1-0.4c-0.1,0-1.4,1.2-1.4,1.2l-2.1-2.9l0.3-0.5l1.8,2.4c0,0,1.4-1.2,1.4-1.2c1.7,0.2,3.4,0.4,5.1,0.4H33.4z'/>",
  "<path class='st8' d='M33.3,36.3h-0.4c-6.9,0-12.6-3.3-12.6-3.6l0.2-0.1l0.2-0.2c3.7,2.1,8,3.3,12.3,3.3h0.4c4.3,0,8.5-1.2,12.3-3.3l0.1,0.2l0.3,0.1C45.9,33,40.2,36.3,33.3,36.3z'/>",
  "<path class='st8' d='M28.2,37.5c-1.7,0-3.4-0.2-5-0.7c-3-1-3.3-3.1-3.3-3.3c0.1-0.2,0.6-1.6,0.7-2l0.5,0.2c-0.3,0.7-0.7,1.9-0.7,1.9l0,0c0,0,0.1,1.8,2.9,2.7c1.8,0.5,3.7,0.8,5.6,0.7l0.5-1.2l0.4,0.2l-0.6,1.5h-0.2C29.1,37.5,28.7,37.5,28.2,37.5z'/>",
  "<path class='st8' d='M27.8,36.6c-1.4,0-2.7-0.2-4-0.7c-2.4-0.9-2.7-2.1-2.7-2.1v-0.1l0.2-0.7l0.5,0.1l-0.1,0.5c0.1,0.2,0.6,1,2.4,1.7c1.4,0.5,2.9,0.7,4.4,0.6l0.2-0.5l0.5,0.2l-0.3,0.8h-0.2C28.5,36.6,28.2,36.6,27.8,36.6z'/>",
  "<polygon class='st8' points='29.1,57.2 28.3,54.7 27.7,50.7 28.2,50.6 28.8,54.6 29.6,57  '/>  ",
  "<path class='st8' d='M21.9,49.5l-0.5-0.2c0,0,0.4-1-0.4-4.4c-0.6-2.5-1.4-4.9-2.5-7.2c-0.2-0.4-0.3-0.7-0.3-0.8l0.5-0.2l0.3,0.8c1.1,2.3,1.9,4.8,2.5,7.3C22.3,48.3,21.9,49.4,21.9,49.5z'/>",
  "<path class='st8' d='M38,37.5c-0.6,0-0.9,0-1,0h-0.2L36.3,36l0.4-0.2l0.5,1.2c1.9,0.1,3.8-0.2,5.6-0.7c2.8-0.9,2.9-2.7,2.9-2.8c0,0-0.5-1.2-0.7-1.9l0.5-0.2c0.2,0.4,0.7,1.9,0.7,2c0,0.1-0.3,2.3-3.3,3.3C41.4,37.3,39.7,37.6,38,37.5z'/>",
  "<path class='st8' d='M38.4,36.6c-0.4,0-0.7,0-0.7,0h-0.2l-0.3-0.8l0.5-0.2l0.2,0.5c1.5,0.1,3-0.1,4.4-0.6c1.8-0.7,2.3-1.5,2.4-1.7l-0.1-0.6l0.5-0.1l0.2,0.7v0.1c0,0-0.4,1.2-2.7,2.1C41.2,36.4,39.8,36.6,38.4,36.6z'/>",
  "<polygon class='st8' points='37,57.2 36.6,57 37.4,54.6 38,50.6 38.5,50.7 37.9,54.7  '/>",
  "<path class='st8' d='M44.3,49.5c0,0-0.4-1.1,0.3-4.7c0.6-2.5,1.5-4.9,2.5-7.3l0.3-0.8l0.5,0.2c-0.1,0.2-0.2,0.4-0.3,0.8c-1.1,2.3-1.9,4.7-2.5,7.2c-0.7,3.4-0.4,4.4-0.4,4.4L44.3,49.5z'/>",
  "<path class='st8' d='M33.2,24.2c-4.3,0-6.9-5.8-7.9-8.1L25,15.6c-0.6-1.5-2.2-2.2-3.7-1.8c-1.4,0.4-2.1,3.1-2.2,4.2L18.5,18c0-0.2,0.6-4.2,2.6-4.8c1.8-0.5,3.8,0.4,4.5,2.1c0.1,0.1,0.1,0.3,0.2,0.5c0.8,1.9,3.4,7.7,7.2,7.7c3.8,0,6.4-5.8,7.3-7.7c0.1-0.2,0.2-0.4,0.3-0.5c0.8-1.7,2.7-2.6,4.5-2.1c2.1,0.6,2.5,4.8,2.5,5L47,18.2c-0.1-1.1-0.7-4-2-4.4c-1.5-0.3-3,0.4-3.7,1.8L41,16.1C40.1,18.1,37.4,24.2,33.2,24.2z'/>",
  "<path class='st8' d='M40.7,55.3c-0.9-0.6-1.9-1.1-2.9-1.6c-0.3,0-2.9,0-4.5,0h-0.4c-1.6,0-4.1-0.1-4.5,0c-1,0.5-2,1-2.9,1.6l-0.3-0.5c1-0.6,2-1.2,3.1-1.7c0.5-0.1,4.2,0,4.6,0h0.4c0.4,0,4.1-0.1,4.6,0c1.1,0.5,2.1,1,3.1,1.7L40.7,55.3z'/>",
  "<path class='st8' d='M48.4,31c0.7-1.1,1.2-2.2,1.6-3.4l-0.1-1.9c-0.3,1.4-0.8,2.8-1.5,4.2c-0.6,0.9-1.3,1.8-2,2.6l-0.5-1.1c0.6-0.7,0.9-1.5,1.1-2.4c0.1-0.8,1.4-9.5,0.3-12.6c-0.1-0.4-0.4-0.7-0.6-1.1l-0.6-0.5c0.2,0.6,0.4,1.2,0.5,1.8c1,3-0.2,11.6-0.3,12.3c0,0.1,0,0.1,0,0.2c-0.3,1.2-1.1,2.3-2.2,2.9c-1.9,1,0,0.6,0.5,0.2c0.2-0.2,0.5-0.3,0.8-0.3l0.4,0.9l0.1,0.3l0,0c0.3,0.8-0.4,3.3-1.2,4.8c-0.6,1-1.4,2-2.1,2.9c-0.9,1.1-1.8,2.3-2.6,3.5c-1.4,2.2-1.9,6.1-1.9,6.3l0.5,0.1c0,0,0.5-4,1.8-6.1c0.8-1.2,1.7-2.4,2.6-3.5c0.8-0.9,1.5-1.9,2.2-2.9c0.8-1.5,1.2-3.1,1.3-4.8c0.5-0.4,0.9-0.9,1.4-1.4c0,0.4-0.1,0.9-0.1,1.1c0,0.7,0,1.1,0.1,1.4c0,0.5,0,1-0.2,1.5c-0.2,0.4-5,8.2-5.6,9.5c-0.6,1.1-0.8,2.4-0.6,3.6c0.5,0.9,0.9,1.9,1.1,2.9l0.7-0.2c-0.3-1.1-0.7-2.1-1.2-3.1c0-0.1,0-0.1,0-0.2c0.2-0.4,0.4-0.8,0.6-1.2c0.3-0.8,0.8-2.6,1.3-3.9c1.6-3,4.2-7.2,4.3-7.4s0.1-1.1,0.1-2.1c0-0.4,0-0.9,0-1.7C48.4,32,48.4,31.5,48.4,31z'/>",
  "<path class='st8' d='M17.8,31c-0.6-1.1-1.2-2.2-1.6-3.4l0.1-1.9c0.3,1.4,0.8,2.8,1.5,4.2c0.6,0.9,1.3,1.8,2,2.6l0.5-1.1c-0.6-0.7-0.9-1.5-1.1-2.4c-0.1-0.8-1.4-9.5-0.3-12.6c0.1-0.4,0.3-0.8,0.6-1.1l0.6-0.5c-0.2,0.6-0.4,1.2-0.5,1.8c-1,3,0.2,11.6,0.3,12.3c0,0.1,0,0.1,0,0.2c0.3,1.2,1.1,2.3,2.2,2.9c1.8,1,0,0.6-0.5,0.2c-0.2-0.2-0.5-0.3-0.8-0.3l-0.4,0.9l-0.1,0.3l0,0c-0.2,0.8,0.4,3.3,1.2,4.8c0.6,1,1.4,2,2.1,2.9c0.9,1.1,1.8,2.3,2.7,3.5c1.4,2.1,1.9,6.1,1.9,6.2l-0.5,0.1c0,0-0.5-4-1.8-6.1c-0.8-1.2-1.7-2.4-2.6-3.5c-0.8-0.9-1.5-1.9-2.2-2.9c-0.8-1.5-1.2-3.1-1.3-4.8c-0.5-0.4-1-0.9-1.4-1.4c0,0.4,0.1,0.9,0.1,1.1c0,0.7-0.1,1.1-0.1,1.4c0,0.5,0.1,1,0.2,1.5c0.2,0.4,5,8.2,5.6,9.5c0.6,1.1,0.8,2.4,0.6,3.6C24.2,50,23.9,51,23.6,52l-0.7-0.2c0.3-1.1,0.7-2.1,1.2-3.1c0-0.1,0-0.1,0-0.2c-0.2-0.4-0.4-0.8-0.6-1.2c-0.3-0.8-0.8-2.6-1.3-3.9c-1.7-2.8-4.3-7.1-4.3-7.2c-0.1-0.1-0.1-1.1-0.1-2.1c0-0.4,0-0.9,0-1.7C17.8,32,17.8,31.5,17.8,31z'/>"
].map(p => SVGtoWKT.convert(p))