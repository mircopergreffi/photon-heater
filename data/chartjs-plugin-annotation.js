/*!
* chartjs-plugin-annotation v1.2.2
* https://www.chartjs.org/chartjs-plugin-annotation/index
 * (c) 2021 chartjs-plugin-annotation Contributors
 * Released under the MIT License
 */
!function(t,e){"object"==typeof exports&&"undefined"!=typeof module?module.exports=e(require("chart.js"),require("chart.js/helpers")):"function"==typeof define&&define.amd?define(["chart.js","chart.js/helpers"],e):(t="undefined"!=typeof globalThis?globalThis:t||self)["chartjs-plugin-annotation"]=e(t.Chart,t.Chart.helpers)}(this,(function(t,e){"use strict";const i=["click","dblclick"],n=["enter","leave"],o=i.concat(n);function r(t,e,i){if(t.listened)switch(e.type){case"mousemove":case"mouseout":!function(t,e){if(!t.moveListened)return;let i;"mousemove"===e.type&&(i=a(t.elements,e));const n=t.hovered;t.hovered=i,function(t,e,i){const{previous:n,element:o}=e;n&&n!==o&&s(n.options.leave||t.listeners.leave,n,i);o&&o!==n&&s(o.options.enter||t.listeners.enter,o,i)}(t,{previous:n,element:i},e)}(t,e);break;case"click":!function(t,e,i){const n=t.listeners,o=a(t.elements,e);if(o){const t=o.options,r=t.dblclick||n.dblclick,a=t.click||n.click;o.clickTimeout?(clearTimeout(o.clickTimeout),delete o.clickTimeout,s(r,o,e)):r?o.clickTimeout=setTimeout((()=>{delete o.clickTimeout,s(a,o,e)}),i.dblClickSpeed):s(a,o,e)}}(t,e,i)}}function s(t,i,n){e.callback(t,[i.$context,n])}function a(t,i){let n=Number.POSITIVE_INFINITY;return t.filter((t=>t.options.display&&t.inRange(i.x,i.y))).reduce(((t,o)=>{const r=o.getCenterPoint(),s=e.distanceBetweenPoints(i,r);return s<n?(t=[o],n=s):s===n&&t.push(o),t}),[]).sort(((t,e)=>t._index-e._index)).slice(0,1)[0]}function l(t,i,n){const o=function(t,i){const n=t.axis,o=t.id,r=n+"ScaleID",s={min:e.valueOrDefault(t.min,Number.NEGATIVE_INFINITY),max:e.valueOrDefault(t.max,Number.POSITIVE_INFINITY)};for(const e of i)e.scaleID===o?c(e,t,["value","endValue"],s):e[r]===o&&c(e,t,[n+"Min",n+"Max",n+"Value"],s);return s}(i,n);let r=d(i,o,"min","suggestedMin");r=d(i,o,"max","suggestedMax")||r,r&&"function"==typeof i.handleTickRangeOptions&&i.handleTickRangeOptions()}function d(t,i,n,o){if(e.isFinite(i[n])&&!function(t,i,n){return e.defined(t[i])||e.defined(t[n])}(t.options,n,o)){const e=t[n]!==i[n];return t[n]=i[n],e}}function h(t,e){for(const i of["scaleID","xScaleID","yScaleID"])t[i]&&!e[t[i]]&&console.warn(`No scale found with id '${t[i]}' for annotation '${t.id}'`)}function c(t,e,i,n){for(const o of i){const i=t[o];if(i){const t=e.parse(i);n.min=Math.min(n.min,t),n.max=Math.max(n.max,t)}}}const u=(t,e,i)=>Math.min(i,Math.max(e,t));function f(t,e,i){for(const n of Object.keys(t))t[n]=u(t[n],e,i);return t}function y(t,e,{x:i,y:n,width:o,height:r}){return t>=i&&t<=i+o&&e>=n&&e<=n+r}function x(t,e){const{x:i,y:n}=t.getProps(["x","y"],e);return{x:i,y:n}}const b=t=>"string"==typeof t&&t.endsWith("%"),g=t=>u(parseFloat(t)/100,0,1);function p(t,e){return"start"===e?0:"end"===e?t:b(e)?g(e)*t:t/2}function m(t,e){return"number"==typeof e?e:b(e)?g(e)*t:t}function v(t){return function(t,i,n){const o={},r=e.isObject(i),s=r?Object.keys(i):i,a=e.isObject(t)?r?n=>e.valueOrDefault(t[n],t[i[n]]):e=>t[e]:()=>t;for(const t of s)o[t]=e.valueOrDefault(a(t),n);return o}(t,["x","y"],"center")}function w(t){return t&&(e.defined(t.xValue)||e.defined(t.yValue))}const M=new Map;function D(t){return t instanceof Image||t instanceof HTMLCanvasElement}function S(t,e){if(e&&e.borderWidth)return t.lineCap=e.borderCapStyle,t.setLineDash(e.borderDash),t.lineDashOffset=e.borderDashOffset,t.lineJoin=e.borderJoinStyle,t.lineWidth=e.borderWidth,t.strokeStyle=e.borderColor,!0}function P(t,i){const n=i.content;if(D(n))return{width:m(n.width,i.width),height:m(n.height,i.height)};const o=e.toFont(i.font),r=e.isArray(n)?n:[n],s=r.join()+o.string+(t._measureText?"-spriting":"");if(!M.has(s)){t.save(),t.font=o.string;const e=r.length;let i=0;for(let n=0;n<e;n++){const e=r[n];i=Math.max(i,t.measureText(e).width)}t.restore();const n=e*o.lineHeight;M.set(s,{width:i,height:n})}return M.get(s)}function C(t,i,n){const{x:o,y:r,width:s,height:a}=i;t.save();const l=S(t,n);t.fillStyle=n.backgroundColor,t.beginPath(),e.addRoundedRectPath(t,{x:o,y:r,w:s,h:a,radius:f(e.toTRBLCorners(e.valueOrDefault(n.cornerRadius,n.borderRadius)),0,Math.min(s,a)/2)}),t.closePath(),t.fill(),l&&t.stroke(),t.restore()}function j(t,i,n){const o=n.content;if(D(o))return void t.drawImage(o,i.x,i.y,i.width,i.height);const r=e.isArray(o)?o:[o],s=e.toFont(n.font),a=s.lineHeight,l=function(t,e){const{x:i,width:n}=t,o=e.textAlign;return"center"===o?i+n/2:"end"===o||"right"===o?i+n:i}(i,n),d=i.y+a/2;t.font=s.string,t.textBaseline="middle",t.textAlign=n.textAlign,t.fillStyle=n.color,r.forEach(((e,i)=>t.fillText(e,l,d+i*a)))}function k(t){const{x:e,y:i,width:n,height:o}=t;return{x:e+n/2,y:i+o/2}}function A(t,i,n){return i="number"==typeof i?i:t.parse(i),e.isFinite(i)?t.getPixelForValue(i):n}function I(t,e){if(t){const i=A(t,e.min,e.start),n=A(t,e.max,e.end);return{start:Math.min(i,n),end:Math.max(i,n)}}return{start:e.start,end:e.end}}function W(t,e){const{chartArea:i,scales:n}=t,o=n[e.xScaleID],r=n[e.yScaleID];let s=i.width/2,a=i.height/2;return o&&(s=A(o,e.xValue,s)),r&&(a=A(r,e.yValue,a)),{x:s,y:a}}function R(t,e){const i=t.scales[e.xScaleID],n=t.scales[e.yScaleID];let{top:o,left:r,bottom:s,right:a}=t.chartArea;if(!i&&!n)return{options:{}};const l=I(i,{min:e.xMin,max:e.xMax,start:r,end:a});r=l.start,a=l.end;const d=I(n,{min:e.yMin,max:e.yMax,start:o,end:s});return o=d.start,s=d.end,{x:r,y:o,x2:a,y2:s,width:a-r,height:s-o}}function T(t,e){if(!w(e)){const i=R(t,e),n=k(i);let o=e.radius;return o&&!isNaN(o)||(o=Math.min(i.width,i.height)/2,e.radius=o),{x:n.x+e.xAdjust,y:n.y+e.yAdjust,width:2*o,height:2*o}}return function(t,e){const i=W(t,e);return{x:i.x+e.xAdjust,y:i.y+e.yAdjust,width:2*e.radius,height:2*e.radius}}(t,e)}class O extends t.Element{inRange(t,e,i){return y(t,e,this.getProps(["x","y","width","height"],i))}getCenterPoint(t){return k(this.getProps(["x","y","width","height"],t))}draw(t){t.save(),C(t,this,this.options),t.restore()}drawLabel(t){const{x:i,y:n,width:o,height:r,options:s}=this,a=s.label;a.borderWidth=s.borderWidth,t.save(),t.beginPath(),t.rect(i+a.borderWidth/2,n+a.borderWidth/2,o-a.borderWidth,r-a.borderWidth),t.clip();const l=v(a.position),d=e.toPadding(a.padding),h=P(t,a);j(t,{x:E(this,h,l,d),y:V(this,h,l,d),width:h.width,height:h.height},a),t.restore()}resolveElementProperties(t,e){return R(t,e)}}function E(t,e,i,n){const{x:o,x2:r,width:s,options:a}=t,{xAdjust:l,borderWidth:d}=a.label;return N({start:o,end:r,size:s},{position:i.x,padding:{start:n.left,end:n.right},adjust:l,borderWidth:d,size:e.width})}function V(t,e,i,n){const{y:o,y2:r,height:s,options:a}=t,{yAdjust:l,borderWidth:d}=a.label;return N({start:o,end:r,size:s},{position:i.y,padding:{start:n.top,end:n.bottom},adjust:l,borderWidth:d,size:e.height})}function N(t,e){const{start:i,end:n}=t,{position:o,padding:{start:r,end:s},adjust:a,borderWidth:l}=e;return i+l/2+a+r+p(n-l-i-r-s-e.size,o)}O.id="boxAnnotation",O.defaults={adjustScaleRange:!0,borderCapStyle:"butt",borderDash:[],borderDashOffset:0,borderJoinStyle:"miter",borderRadius:0,borderWidth:1,cornerRadius:void 0,display:!0,label:{color:"black",content:null,drawTime:void 0,enabled:!1,font:{family:void 0,lineHeight:void 0,size:void 0,style:void 0,weight:"bold"},height:void 0,padding:6,position:"center",textAlign:"start",xAdjust:0,yAdjust:0,width:void 0},xMax:void 0,xMin:void 0,xScaleID:"x",yMax:void 0,yMin:void 0,yScaleID:"y"},O.defaultRoutes={borderColor:"color",backgroundColor:"color"};const L=Math.PI,z=(t,e,i)=>({x:t.x+i*(e.x-t.x),y:t.y+i*(e.y-t.y)}),H=(t,e,i)=>z(e,i,Math.abs((t-e.y)/(i.y-e.y))).x,Y=(t,e,i)=>z(e,i,Math.abs((t-e.x)/(i.x-e.x))).y,_=t=>t*t;function X({x:t,y:e,x2:i,y2:n},{top:o,right:r,bottom:s,left:a}){return!(t<a&&i<a||t>r&&i>r||e<o&&n<o||e>s&&n>s)}function F({x:t,y:e},i,{top:n,right:o,bottom:r,left:s}){return t<s&&(e=Y(s,{x:t,y:e},i),t=s),t>o&&(e=Y(o,{x:t,y:e},i),t=o),e<n&&(t=H(n,{x:t,y:e},i),e=n),e>r&&(t=H(r,{x:t,y:e},i),e=r),{x:t,y:e}}class $ extends t.Element{intersects(t,e,i=.001,n){const{x:o,y:r,x2:s,y2:a}=this.getProps(["x","y","x2","y2"],n),l=s-o,d=a-r,h=_(l)+_(d),c=0===h?-1:((t-o)*l+(e-r)*d)/h;let u,f;return c<0?(u=o,f=r):c>1?(u=s,f=a):(u=o+c*l,f=r+c*d),_(t-u)+_(e-f)<i}labelIsVisible(t,e){const i=this.options.label;return!!(e&&i&&i.enabled)&&X(this.getProps(["x","y","x2","y2"],t),e)}isOnLabel(t,e,i){if(!this.labelIsVisible(i))return!1;const{labelX:n,labelY:o,labelWidth:r,labelHeight:s,labelRotation:a}=this.getProps(["labelX","labelY","labelWidth","labelHeight","labelRotation"],i),{x:l,y:d}=(h={x:t,y:e},c={x:n,y:o},u=-a,f=Math.cos(u),y=Math.sin(u),x=c.x,b=c.y,{x:x+f*(h.x-x)-y*(h.y-b),y:b+y*(h.x-x)+f*(h.y-b)});var h,c,u,f,y,x,b;const g=r/2,p=s/2;return l>=n-g&&l<=n+g&&d>=o-p&&d<=o+p}inRange(t,e,i){const n=_(this.options.borderWidth/2);return this.intersects(t,e,n,i)||this.isOnLabel(t,e,i)}getCenterPoint(){return{x:(this.x2+this.x)/2,y:(this.y2+this.y)/2}}draw(t){const{x:e,y:i,x2:n,y2:o,options:r}=this;t.save(),t.lineWidth=r.borderWidth,t.strokeStyle=r.borderColor,t.setLineDash(r.borderDash),t.lineDashOffset=r.borderDashOffset,t.beginPath(),t.moveTo(e,i),t.lineTo(n,o),t.stroke(),t.restore()}drawLabel(t,e){if(!this.labelIsVisible(!1,e))return;const{labelX:i,labelY:n,labelWidth:o,labelHeight:r,labelRotation:s,labelPadding:a,labelTextSize:l,options:{label:d}}=this;t.save(),t.translate(i,n),t.rotate(s);C(t,{x:-o/2,y:-r/2,width:o,height:r},d);j(t,{x:-o/2+a.left+d.borderWidth/2,y:-r/2+a.top+d.borderWidth/2,width:l.width,height:l.height},d),t.restore()}resolveElementProperties(t,i){const n=t.scales[i.scaleID];let o,r,{top:s,left:a,bottom:l,right:d}=t.chartArea;if(n)o=A(n,i.value,NaN),r=A(n,i.endValue,o),n.isHorizontal()?(a=o,d=r):(s=o,l=r);else{const e=t.scales[i.xScaleID],n=t.scales[i.yScaleID];e&&(a=A(e,i.xMin,a),d=A(e,i.xMax,d)),n&&(s=A(n,i.yMin,s),l=A(n,i.yMax,l))}const h=X({x:a,y:s,x2:d,y2:l},t.chartArea)?function(t,e,i){const{x:n,y:o}=F(t,e,i),{x:r,y:s}=F(e,t,i);return{x:n,y:o,x2:r,y2:s,width:Math.abs(r-n),height:Math.abs(s-o)}}({x:a,y:s},{x:d,y:l},t.chartArea):{x:a,y:s,x2:d,y2:l,width:Math.abs(d-a),height:Math.abs(l-s)},c=i.label;return c&&c.content?function(t,i,n){const{padding:o,xPadding:r,yPadding:s,borderWidth:a}=n,l=function(t,i,n){let o=t;(i||n)&&(o={x:i||6,y:n||6});return e.toPadding(o)}(o,r,s),d=P(i.ctx,n),h=d.width+l.width+a,c=d.height+l.height+a,u=function(t,i,n,o){const{width:r,height:s,padding:a}=n,{xAdjust:l,yAdjust:d}=i,h={x:t.x,y:t.y},c={x:t.x2,y:t.y2},u="auto"===i.rotation?function(t){const{x:e,y:i,x2:n,y2:o}=t,r=Math.atan2(o-i,n-e);return r>L/2?r-L:r<L/-2?r+L:r}(t):e.toRadians(i.rotation),f=function(t,e,i){const n=Math.cos(i),o=Math.sin(i);return{w:Math.abs(t*n)+Math.abs(e*o),h:Math.abs(t*o)+Math.abs(e*n)}}(r,s,u),y=function(t,e,i,n){let o;const r=function(t,e){const{x:i,x2:n,y:o,y2:r}=t,s=Math.min(o,r)-e.top,a=Math.min(i,n)-e.left,l=e.bottom-Math.max(o,r),d=e.right-Math.max(i,n);return{x:Math.min(a,d),y:Math.min(s,l),dx:a<=d?1:-1,dy:s<=l?1:-1}}(t,n);o="start"===e.position?J({w:t.x2-t.x,h:t.y2-t.y},i,e,r):"end"===e.position?1-J({w:t.x-t.x2,h:t.y-t.y2},i,e,r):p(1,e.position);return o}(t,i,{labelSize:f,padding:a},o),x=z(h,c,y),b={size:f.w,min:o.left,max:o.right,padding:a.left},g={size:f.h,min:o.top,max:o.bottom,padding:a.top};return{x:U(x.x,b)+l,y:U(x.y,g)+d,width:r,height:s,rotation:u}}(t,n,{width:h,height:c,padding:l},i.chartArea);return t.labelX=u.x,t.labelY=u.y,t.labelWidth=u.width,t.labelHeight=u.height,t.labelRotation=u.rotation,t.labelPadding=l,t.labelTextSize=d,t}(h,t,c):h}}function J(t,e,i,n){const{labelSize:o,padding:r}=e,s=t.w*n.dx,a=t.h*n.dy,l=s>0&&(o.w/2+r.left-n.x)/s,d=a>0&&(o.h/2+r.top-n.y)/a;return u(Math.max(l,d),0,.25)}function U(t,e){const{size:i,min:n,max:o,padding:r}=e,s=i/2;return i>o-n?(o+n)/2:(n>=t-r-s&&(t=n+r+s),o<=t+r+s&&(t=o-r-s),t)}$.id="lineAnnotation",$.defaults={adjustScaleRange:!0,borderDash:[],borderDashOffset:0,borderWidth:2,display:!0,endValue:void 0,label:{backgroundColor:"rgba(0,0,0,0.8)",borderCapStyle:"butt",borderColor:"black",borderDash:[],borderDashOffset:0,borderJoinStyle:"miter",borderRadius:6,borderWidth:0,color:"#fff",content:null,cornerRadius:void 0,drawTime:void 0,enabled:!1,font:{family:void 0,lineHeight:void 0,size:void 0,style:void 0,weight:"bold"},height:void 0,padding:6,position:"center",rotation:0,textAlign:"center",width:void 0,xAdjust:0,xPadding:void 0,yAdjust:0,yPadding:void 0},scaleID:void 0,value:void 0,xMax:void 0,xMin:void 0,xScaleID:"x",yMax:void 0,yMin:void 0,yScaleID:"y"},$.defaultRoutes={borderColor:"color"};class B extends t.Element{inRange(t,i,n){return function(t,i,n){const{width:o,height:r}=i,s=i.getCenterPoint(!0),a=o/2,l=r/2;if(a<=0||l<=0)return!1;const d=e.toRadians(n||0),h=Math.cos(d),c=Math.sin(d),u=Math.pow(h*(t.x-s.x)+c*(t.y-s.y),2),f=Math.pow(c*(t.x-s.x)-h*(t.y-s.y),2);return u/Math.pow(a,2)+f/Math.pow(l,2)<=1}({x:t,y:i},this.getProps(["x","y","width","height"],n),this.options.rotation)}getCenterPoint(t){return k(this.getProps(["x","y","width","height"],t))}draw(t){const{width:i,height:n,options:o}=this,r=this.getCenterPoint();t.save(),t.translate(r.x,r.y),o.rotation&&t.rotate(e.toRadians(o.rotation)),t.beginPath(),t.lineWidth=o.borderWidth,t.strokeStyle=o.borderColor,t.fillStyle=o.backgroundColor,t.setLineDash(o.borderDash),t.lineDashOffset=o.borderDashOffset,t.ellipse(0,0,n/2,i/2,Math.PI/2,0,2*Math.PI),t.fill(),t.stroke(),t.restore()}resolveElementProperties(t,e){return R(t,e)}}B.id="ellipseAnnotation",B.defaults={adjustScaleRange:!0,borderDash:[],borderDashOffset:0,borderWidth:1,display:!0,rotation:0,xMax:void 0,xMin:void 0,xScaleID:"x",yMax:void 0,yMin:void 0,yScaleID:"y"},B.defaultRoutes={borderColor:"color",backgroundColor:"color"};class q extends t.Element{inRange(t,e,i){return y(t,e,this.getProps(["x","y","width","height"],i))}getCenterPoint(t){return k(this.getProps(["x","y","width","height"],t))}draw(t){if(!this.options.content)return;const{labelX:e,labelY:i,labelWidth:n,labelHeight:o,options:r}=this;!function(t,e){const{pointX:i,pointY:n,calloutPosition:o,options:r}=e;if(!o)return;const s=r.callout,{separatorStart:a,separatorEnd:l}=function(t,e){const{x:i,y:n,width:o,height:r}=t,s=function(t,e){const{width:i,height:n,options:o}=t,r=o.callout.margin+o.borderWidth/2;if("right"===e)return i+r;if("bottom"===e)return n+r;return-r}(t,e);let a,l;"left"===e||"right"===e?(a={x:i+s,y:n},l={x:a.x,y:a.y+r}):"top"!==e&&"bottom"!==e||(a={x:i,y:n+s},l={x:a.x+o,y:a.y});return{separatorStart:a,separatorEnd:l}}(e,o),{sideStart:d,sideEnd:h}=function(t,e,i){const{y:n,width:o,height:r,options:s}=t,a=s.callout.start,l=function(t,e){const i=e.side;if("left"===t||"top"===t)return-i;return i}(e,s.callout);let d,h;"left"===e||"right"===e?(d={x:i.x,y:n+m(r,a)},h={x:d.x+l,y:d.y}):"top"!==e&&"bottom"!==e||(d={x:i.x+m(o,a),y:i.y},h={x:d.x,y:d.y+l});return{sideStart:d,sideEnd:h}}(e,o,a);t.save(),t.beginPath();const c=S(t,s);(s.margin>0||0===r.borderWidth)&&(t.moveTo(a.x,a.y),t.lineTo(l.x,l.y));t.moveTo(d.x,d.y),t.lineTo(h.x,h.y),t.lineTo(i,n),c&&t.stroke();t.restore()}(t,this),this.boxVisible&&C(t,this,r),j(t,{x:e,y:i,width:n,height:o},r)}resolveElementProperties(t,i){const n=w(i)?W(t,i):k(R(t,i)),o=e.toPadding(i.padding),r=P(t.ctx,i),s=function(t,e,i,n){const o=e.width+n.width+i.borderWidth,r=e.height+n.height+i.borderWidth,s=v(i.position);return{x:G(t.x,o,i.xAdjust,s.x),y:G(t.y,r,i.yAdjust,s.y),width:o,height:r}}(n,r,i,o),a=e.color(i.backgroundColor),l={boxVisible:i.borderWidth>0||a&&a.valid&&a.rgb.a>0,pointX:n.x,pointY:n.y,...s,labelX:s.x+o.left+i.borderWidth/2,labelY:s.y+o.top+i.borderWidth/2,labelWidth:r.width,labelHeight:r.height};return l.calloutPosition=i.callout.enabled&&function(t,e){const i=e.position;if("left"===i||"right"===i||"top"===i||"bottom"===i)return i;return function(t,e){const{x:i,y:n,width:o,height:r,pointX:s,pointY:a}=t,{margin:l,side:d}=e,h=l+d;if(s<i-h)return"left";if(s>i+o+h)return"right";if(a<n+r+h)return"top";if(a>n-h)return"bottom"}(t,e)}(l,i.callout),l}}function G(t,e,i=0,n){return t-p(e,n)+i}q.id="labelAnnotation",q.defaults={adjustScaleRange:!0,backgroundColor:"transparent",borderCapStyle:"butt",borderDash:[],borderDashOffset:0,borderJoinStyle:"miter",borderRadius:0,borderWidth:0,callout:{borderCapStyle:"butt",borderColor:void 0,borderDash:[],borderDashOffset:0,borderJoinStyle:"miter",borderWidth:1,enabled:!1,margin:5,position:"auto",side:5,start:"50%"},color:"black",content:null,display:!0,font:{family:void 0,lineHeight:void 0,size:void 0,style:void 0,weight:void 0},height:void 0,padding:6,position:"center",textAlign:"center",width:void 0,xAdjust:0,xMax:void 0,xMin:void 0,xScaleID:"x",xValue:void 0,yAdjust:0,yMax:void 0,yMin:void 0,yScaleID:"y",yValue:void 0},q.defaultRoutes={borderColor:"color",backgroundColor:"color"};class K extends t.Element{inRange(t,e,i){const{width:n}=this.getProps(["width"],i);return o={x:t,y:e},r=this.getCenterPoint(i),s=n/2+this.options.borderWidth,!(!o||!r||s<=0)&&Math.pow(o.x-r.x,2)+Math.pow(o.y-r.y,2)<=Math.pow(s,2);var o,r,s}getCenterPoint(t){return x(this,t)}draw(t){const i=this.options;t.save(),t.fillStyle=i.backgroundColor,S(t,i),e.drawPoint(t,i,this.x,this.y),t.restore()}resolveElementProperties(t,e){return T(t,e)}}K.id="pointAnnotation",K.defaults={adjustScaleRange:!0,borderDash:[],borderDashOffset:0,borderWidth:1,display:!0,pointStyle:"circle",radius:10,rotation:0,xAdjust:0,xMax:void 0,xMin:void 0,xScaleID:"x",xValue:void 0,yAdjust:0,yMax:void 0,yMin:void 0,yScaleID:"y",yValue:void 0},K.defaultRoutes={borderColor:"color",backgroundColor:"color"};class Q extends t.Element{inRange(t,e,i){const n=Z(this.getProps(["x","y"],i),this.options);return n&&n.length>0&&function(t,e,i){let n=!1,o=0,r=t.length-1;for(;o<t.length;r=o++)t[o].y>i!=t[r].y>i&&e<(t[r].x-t[o].x)*(i-t[o].y)/(t[r].y-t[o].y)+t[o].x&&(n=!n);return n}(n,t,e)}getCenterPoint(t){return x(this,t)}draw(t){const{x:e,y:i,options:n}=this,o=Z({x:e,y:i},n);let r=o[0];t.save(),t.beginPath(),t.fillStyle=n.backgroundColor;const s=S(t,n);t.moveTo(r.x,r.y);for(let e=1;e<o.length;e++)r=o[e],t.lineTo(r.x,r.y);t.closePath(),t.fill(),s&&t.stroke(),t.restore()}resolveElementProperties(t,i){return e.isNumber(i.sides)&&i.sides>=1?T(t,i):{options:{}}}}function Z(t,i){const{sides:n,radius:o}=i;let r=2*e.PI/n,s=i.rotation*e.RAD_PER_DEG;const a=new Array;tt(a,t,s,o);for(let e=0;e<n;e++)s+=r,tt(a,t,s,o);return a}function tt(t,e,i,n){t.push({x:e.x+Math.sin(i)*n,y:e.y-Math.cos(i)*n})}Q.id="polygonAnnotation",Q.defaults={adjustScaleRange:!0,borderCapStyle:"butt",borderDash:[],borderDashOffset:0,borderJoinStyle:"miter",borderWidth:1,display:!0,radius:10,rotation:0,sides:3,xAdjust:0,xMax:void 0,xMin:void 0,xScaleID:"x",xValue:void 0,yAdjust:0,yMax:void 0,yMin:void 0,yScaleID:"y",yValue:void 0},Q.defaultRoutes={borderColor:"color",backgroundColor:"color"};const et={box:O,ellipse:B,label:q,line:$,point:K,polygon:Q};Object.keys(et).forEach((e=>{t.defaults.describe(`elements.${et[e].id}`,{_fallback:"plugins.annotation"})}));const it=new Map,nt=t.Chart.version.split(".");var ot={id:"annotation",version:"1.2.2",afterRegister(){t.Chart.register(et),"3"===nt[0]&&parseInt(nt[1],10)<=6&&t.Chart.defaults.set("elements.lineAnnotation",{callout:{},font:{},padding:6})},afterUnregister(){t.Chart.unregister(et)},beforeInit(t){it.set(t,{annotations:[],elements:[],visibleElements:[],listeners:{},listened:!1,moveListened:!1})},beforeUpdate(t,i,n){const o=it.get(t).annotations=[];let r=n.annotations;e.isObject(r)?Object.keys(r).forEach((t=>{const i=r[t];e.isObject(i)&&(i.id=t,o.push(i))})):e.isArray(r)&&o.push(...r),function(t,e){for(const i of t)h(i,e)}(o,t.scales)},afterDataLimits(t,e){const i=it.get(t);l(0,e.scale,i.annotations.filter((t=>t.display&&t.adjustScaleRange)))},afterUpdate(r,s,a){const l=it.get(r);!function(t,r,s){const a=r.annotations||[];r.listened=!1,r.moveListened=!1,o.forEach((t=>{"function"==typeof s[t]?(r.listened=!0,r.listeners[t]=s[t]):e.defined(r.listeners[t])&&delete r.listeners[t]})),n.forEach((t=>{"function"==typeof s[t]&&(r.moveListened=!0)})),r.listened&&r.moveListened||a.forEach((t=>{r.listened||i.forEach((e=>{"function"==typeof t[e]&&(r.listened=!0)})),r.moveListened||n.forEach((e=>{"function"==typeof t[e]&&(r.listened=!0,r.moveListened=!0)}))}))}(0,l,a),function(e,i,n,o){const r=function(e,i,n){if("reset"===n||"none"===n||"resize"===n)return rt;return new t.Animations(e,i)}(e,n.animations,o),s=i.annotations,a=function(t,e){const i=e.length,n=t.length;if(n<i){const e=i-n;t.splice(n,0,...new Array(e))}else n>i&&t.splice(i,n-i);return t}(i.elements,s);for(let t=0;t<s.length;t++){const i=s[t];let n=a[t];const o=et[st(i.type)];n&&n instanceof o||(n=a[t]=new o);const l=at(i.setContext(dt(e,n,i))),d=n.resolveElementProperties(e,l);d.skip=isNaN(d.x)||isNaN(d.y),d.options=l,r.update(n,d)}}(r,l,a,s.mode),l.visibleElements=l.elements.filter((t=>!t.skip&&t.options.display))},beforeDatasetsDraw(t,e,i){ht(t,"beforeDatasetsDraw",i.clip)},afterDatasetsDraw(t,e,i){ht(t,"afterDatasetsDraw",i.clip)},beforeDraw(t,e,i){ht(t,"beforeDraw",i.clip)},afterDraw(t,e,i){ht(t,"afterDraw",i.clip)},beforeEvent(t,e,i){r(it.get(t),e.event,i)},destroy(t){it.delete(t)},_getState:t=>it.get(t),defaults:{animations:{numbers:{properties:["x","y","x2","y2","width","height","pointX","pointY","labelX","labelY","labelWidth","labelHeight","radius"],type:"number"}},clip:!0,dblClickSpeed:350,drawTime:"afterDatasetsDraw",label:{drawTime:null}},descriptors:{_indexable:!1,_scriptable:t=>!o.includes(t),annotations:{_allKeys:!1,_fallback:(t,e)=>`elements.${et[st(e.type)].id}`}},additionalOptionScopes:[""]};const rt={update:Object.assign};function st(t="line"){return et[t]?t:(console.warn(`Unknown annotation type: '${t}', defaulting to 'line'`),"line")}function at(t){const e=et[st(t.type)],i={};i.id=t.id,i.type=t.type,i.drawTime=t.drawTime,Object.assign(i,lt(t,e.defaults),lt(t,e.defaultRoutes));for(const e of o)i[e]=t[e];return i}function lt(t,i){const n={};for(const o of Object.keys(i)){const r=i[o],s=t[o];n[o]=e.isObject(r)?lt(s,r):s}return n}function dt(t,e,i){return e.$context||(e.$context=Object.assign(Object.create(t.getContext()),{element:e,id:i.id,type:"annotation"}))}function ht(t,i,n){const{ctx:o,chartArea:r}=t,s=it.get(t);n&&e.clipArea(o,r),s.visibleElements.forEach((t=>{t.options.drawTime===i&&t.draw(o)})),n&&e.unclipArea(o),s.visibleElements.forEach((t=>{if(!("drawLabel"in t))return;const e=t.options.label;e&&e.enabled&&e.content&&(e.drawTime||t.options.drawTime)===i&&t.drawLabel(o,r)}))}return t.Chart.register(ot),ot}));