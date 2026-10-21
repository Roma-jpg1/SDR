// Deterministic eight-second orbit with locally generated geometry and textures.
export function createScene(T) {
  const TAU = Math.PI * 2;
  const renderer = new T.WebGLRenderer({ canvas: document.querySelector('#space'), alpha: true, antialias: true });
  renderer.setSize(1200, 400, false);
  renderer.setPixelRatio(1);
  renderer.outputColorSpace = T.SRGBColorSpace;
  renderer.toneMapping = T.ACESFilmicToneMapping;
  renderer.toneMappingExposure = 1.25;
  const scene = new T.Scene();
  const camera = new T.OrthographicCamera(-6, 6, 2, -2, .1, 100);
  camera.position.set(0, 0, 12);
  scene.add(new T.AmbientLight(0x8bb7db, 1.6));
  const sun = new T.DirectionalLight(0xd7f4ff, 3.8);
  sun.position.set(-4, 6, 7);
  scene.add(sun);
  const rim = new T.DirectionalLight(0x2b90f5, 2.4);
  rim.position.set(5, -1, -4);
  scene.add(rim);

  let seed = 43821;
  const rand = () => { seed = (Math.imul(seed, 1664525) + 1013904223) >>> 0; return seed / 4294967296; };
  const stars = [];
  for (let i = 0; i < 125; i++) {
    const x = rand() * 12 - 6, y = rand() * 4 - 2;
    if (x < -.3 && y > -1.5 && y < 1.5) continue;
    stars.push(x, y, -8);
  }
  const starGeometry = new T.BufferGeometry();
  starGeometry.setAttribute('position', new T.Float32BufferAttribute(stars, 3));
  scene.add(new T.Points(starGeometry, new T.PointsMaterial({ color: 0x93b6d3, size: .012, transparent: true, opacity: .58 })));
  const world = new T.Group();
  world.position.set(2.9, 0, 0);
  scene.add(world);

  // Hand-drawn geographic silhouettes on an equirectangular texture.
  const continents = [
    [[-17,36],[-5,36],[10,37],[24,32],[32,31],[35,23],[43,13],[51,11],[44,1],[42,-11],[35,-18],[32,-28],[18,-35],[11,-27],[11,-16],[6,-3],[-3,5],[-12,5],[-17,15],[-16,24]],
    [[-10,36],[-9,44],[-1,43],[-5,49],[3,51],[7,57],[8,63],[20,71],[32,70],[28,61],[40,65],[55,69],[70,72],[94,77],[116,73],[138,72],[158,64],[179,65],[173,53],[156,51],[147,45],[140,35],[132,35],[128,40],[122,39],[121,30],[112,22],[107,19],[110,5],[104,1],[98,10],[93,21],[89,22],[80,7],[75,9],[70,22],[61,25],[56,26],[52,17],[44,13],[40,20],[35,30],[29,36],[27,41],[20,40],[17,44],[12,45],[15,38],[10,39],[7,44],[0,42],[-2,37]],
    [[-168,71],[-150,70],[-140,61],[-132,56],[-125,49],[-124,40],[-117,32],[-110,29],[-105,22],[-98,17],[-88,16],[-83,10],[-78,9],[-82,20],[-87,21],[-90,29],[-81,25],[-81,32],[-75,36],[-70,44],[-61,48],[-54,53],[-63,59],[-78,62],[-84,70],[-105,74],[-125,71],[-144,68]],
    [[-81,12],[-72,12],[-62,9],[-52,4],[-49,-1],[-35,-6],[-38,-16],[-42,-23],[-50,-28],[-55,-38],[-66,-55],[-73,-50],[-75,-35],[-71,-17],[-79,-5],[-81,4]],
    [[113,-22],[114,-29],[120,-34],[132,-34],[138,-36],[146,-39],[153,-29],[153,-23],[145,-15],[142,-10],[136,-13],[129,-14],[123,-17],[117,-20]],
    [[-54,59],[-43,60],[-20,69],[-18,79],[-42,84],[-60,80],[-73,72]],
    [[44,-12],[50,-15],[49,-24],[44,-26],[43,-19]],
    [[-8,50],[-5,58],[-2,59],[1,53]],
    [[130,32],[136,36],[140,41],[145,44],[143,37],[137,33]],
    [[96,5],[105,-5],[102,-6],[94,3]],
    [[109,7],[118,6],[117,-3],[110,-4]],
    [[130,-2],[141,-3],[151,-8],[143,-10],[134,-5]],
    [[172,-34],[177,-38],[174,-42],[168,-47],[166,-45],[171,-40]]
  ];
  const map = document.createElement('canvas'); map.width = 2048; map.height = 1024;
  const ctx = map.getContext('2d');
  ctx.fillStyle = '#123f67'; ctx.fillRect(0, 0, 2048, 1024);
  function path(coords) {
    ctx.beginPath();
    coords.forEach(([lon, lat], i) => { const x = (lon + 180) / 360 * 2048, y = (90 - lat) / 180 * 1024; i ? ctx.lineTo(x, y) : ctx.moveTo(x, y); });
    ctx.closePath();
  }
  for (const shape of continents) {
    path(shape);
    ctx.fillStyle = '#3e969e'; ctx.fill();
    ctx.strokeStyle = '#83c8cc'; ctx.lineWidth = 1.9; ctx.stroke();
    ctx.save(); ctx.clip();
    for (let i = 0; i < 1500; i++) {
      ctx.fillStyle = i % 2 ? '#9ce9cc12' : '#082f4515';
      ctx.beginPath(); ctx.ellipse(rand()*2048,rand()*1024,rand()*16+2,rand()*8+2,0,0,TAU); ctx.fill();
    }
    ctx.restore();
  }
  ctx.lineWidth = .8; ctx.strokeStyle = '#8fcde525';
  for (let lon = 0; lon < 360; lon += 15) { ctx.beginPath(); ctx.moveTo(lon/360*2048,0); ctx.lineTo(lon/360*2048,1024); ctx.stroke(); }
  for (let lat = 0; lat < 180; lat += 15) { ctx.beginPath(); ctx.moveTo(0,lat/180*1024); ctx.lineTo(2048,lat/180*1024); ctx.stroke(); }
  const texture = new T.CanvasTexture(map); texture.colorSpace = T.SRGBColorSpace; texture.anisotropy = 8;
  const globe = new T.Mesh(new T.SphereGeometry(1.16, 96, 64), new T.MeshStandardMaterial({ map: texture, roughness: .87, metalness: .04 }));
  globe.rotation.set(.1, -1.73, -.12);
  world.add(globe);
  const atmosphere = new T.Mesh(new T.SphereGeometry(1.22, 64, 48), new T.ShaderMaterial({
    transparent: true, depthWrite: false, side: T.BackSide, blending: T.AdditiveBlending,
    vertexShader: 'varying vec3 n; varying vec3 v; void main(){ vec4 p=modelViewMatrix*vec4(position,1.); n=normalize(normalMatrix*normal); v=normalize(-p.xyz); gl_Position=projectionMatrix*p; }',
    fragmentShader: 'varying vec3 n; varying vec3 v; void main(){ float f=pow(1.-abs(dot(normalize(n),normalize(v))),3.); gl_FragColor=vec4(.15,.60,1.,f*.46); }'
  }));
  world.add(atmosphere);

  const gold = new T.MeshStandardMaterial({ color: 0xd8b578, roughness: .38, metalness: .58 });
  const silver = new T.MeshStandardMaterial({ color: 0xe1eaf1, roughness: .3, metalness: .4 });
  const dark = new T.MeshStandardMaterial({ color: 0x273749, roughness: .55, metalness: .4 });
  const solarCanvas = document.createElement('canvas'); solarCanvas.width = 256; solarCanvas.height = 128;
  const sc = solarCanvas.getContext('2d');
  sc.fillStyle = '#6e9ab4'; sc.fillRect(0,0,256,128);
  for(let row=0;row<4;row++) for(let col=0;col<8;col++) {
    sc.fillStyle = (row+col)%3 === 0 ? '#24508b' : '#15376c';
    sc.fillRect(col*32+2,row*32+2,28,28);
    sc.fillStyle = '#5b86aa'; sc.fillRect(col*32+4,row*32+15,24,1);
  }
  const solarTex = new T.CanvasTexture(solarCanvas); solarTex.colorSpace=T.SRGBColorSpace;
  const solarMat = new T.MeshStandardMaterial({ map:solarTex, metalness:.4,roughness:.37,side:T.DoubleSide });
  function box(parent,w,h,d,material,x=0,y=0,z=0) {
    const mesh=new T.Mesh(new T.BoxGeometry(w,h,d),material); mesh.position.set(x,y,z);parent.add(mesh);return mesh;
  }
  function satellite(scale) {
    const sat = new T.Group(); sat.scale.setScalar(scale);
    box(sat,.31,.36,.27,gold);
    box(sat,.33,.04,.29,silver,0,.2);
    box(sat,.33,.035,.29,dark,0,-.2);
    box(sat,1.32,.035,.035,silver,0,0,-.03);
    for(const sign of [-1,1]) {
      box(sat,.51,.35,.035,dark,sign*.48,0,-.03);
      const panel = new T.Mesh(new T.PlaneGeometry(.5,.34),solarMat);panel.position.set(sign*.48,0,-.009);sat.add(panel);
      const rear=panel.clone();rear.position.z=-.05;rear.rotation.y=Math.PI;sat.add(rear);
    }
    const profile=[];for(let i=0;i<=14;i++){const r=i/14*.14;profile.push(new T.Vector2(r,2.7*r*r));}
    const dish=new T.Mesh(new T.LatheGeometry(profile,40),new T.MeshStandardMaterial({color:0xe7f3fa,metalness:.3,roughness:.3,side:T.DoubleSide}));
    dish.rotation.x=Math.PI/2;dish.position.set(0,.05,.22);sat.add(dish);
    const feed=new T.Mesh(new T.CylinderGeometry(.008,.008,.19,8),silver);feed.rotation.x=Math.PI/2;feed.position.set(0,.05,.27);sat.add(feed);
    const receiver=new T.Mesh(new T.SphereGeometry(.024,12,8),gold);receiver.position.set(0,.05,.37);sat.add(receiver);
    const antenna=new T.Mesh(new T.CylinderGeometry(.006,.006,.25,6),silver);antenna.position.set(.11,.35,0);sat.add(antenna);
    const tip=new T.Mesh(new T.SphereGeometry(.014,8,8),new T.MeshBasicMaterial({color:0x84f7ec}));tip.position.set(.11,.48,0);sat.add(tip);
    world.add(sat);return sat;
  }
  const paths = [
    { radius:2.04, tilt:1.15, roll:.25, phase:.55, direction:1, craft:satellite(.79), color:0x69cce5 },
    { radius:1.75, tilt:.95, roll:-.68, phase:3.5, direction:-1, craft:satellite(.59), color:0x7e96d9 }
  ];
  function orbitPoint(path,angle) {
    return new T.Vector3(Math.cos(angle)*path.radius,Math.sin(angle)*path.radius,0)
      .applyEuler(new T.Euler(path.tilt,0,path.roll,'ZXY'));
  }
  for(const path of paths) {
    const pts=Array.from({length:241},(_,i)=>orbitPoint(path,i/240*TAU));
    world.add(new T.Line(new T.BufferGeometry().setFromPoints(pts),new T.LineBasicMaterial({color:path.color,transparent:true,opacity:.24})));
    const points=new Float32Array(65*3),colors=new Float32Array(65*3),color=new T.Color(path.color);
    for(let i=0;i<65;i++){const k=(i/64)**1.5;colors[i*3]=color.r*k;colors[i*3+1]=color.g*k;colors[i*3+2]=color.b*k;}
    const trailGeometry=new T.BufferGeometry();trailGeometry.setAttribute('position',new T.BufferAttribute(points,3));trailGeometry.setAttribute('color',new T.BufferAttribute(colors,3));
    const trail=new T.Line(trailGeometry,new T.LineBasicMaterial({vertexColors:true,transparent:true,opacity:.9,blending:T.AdditiveBlending,depthWrite:false}));
    path.trail=trail;world.add(trail);
  }
  const wave=document.querySelector('#wave');
  function renderAt(time) {
    const phase=time/8*TAU;
    for(const path of paths) {
      const a=phase*path.direction+path.phase;
      path.craft.position.copy(orbitPoint(path,a));
      path.craft.rotation.set(.3+.14*Math.cos(a),-.22+.16*Math.sin(a),a+.3);
      const positions=path.trail.geometry.attributes.position;
      for(let i=0;i<65;i++){const v=orbitPoint(path,a-path.direction*(1-i/64)*.65);positions.setXYZ(i,v.x,v.y,v.z);}
      positions.needsUpdate=true;
    }
    let d='';
    for(let x=0;x<=455;x+=2) {
      const envelope=Math.exp(-(((x-230)/130)**2))*Math.sin(x/455*Math.PI)**2;
      const y=22.5+Math.sin(x*.105-phase*2)*14*envelope;
      d+=(x?'L':'M')+x+','+y.toFixed(2);
    }
    wave.setAttribute('d',d);
    renderer.render(scene,camera);
  }
  return renderAt;
}
