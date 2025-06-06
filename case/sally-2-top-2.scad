use <C:/Users/rolan/Downloads/Atarian/SF Atarian System.ttf>

thick = 2;
xmax = 250;
ymax = 180+thick;
zmax = 39;

edge=5;

//translate([230,15,0])
//    ataritext();

difference(){
    translate([235,17,0])
        ataritext();
}

module atarilogo() {
    translate([0,32.5,0])
        base_shape_a();
}

module ataritext() {
    linear_extrude(0.2)
        mirror([1,0,0])
        text("ABBUC", 10, "SF Atarian System:style=Regular");

    translate([-40, 1, 0])
    linear_extrude(0.2)
        mirror([1,0,0])
    text("Sally 2 ", 8
    , "Brush Script MT:style=Italic"  );
 
}


module base_shape_a() {
        rotate([90,0,0]) 
            linear_extrude(20)
                base_shape_2d_a(0.4);
}


module base_shape_2d_a(th) {
    polygon([/* [0, zmax], */ [0, edge], [edge,0], [xmax-edge, 0], [xmax, edge], /*[xmax, zmax],[xmax-th,zmax],*/ [xmax-th, edge+th], [xmax-edge-th, th],[edge+th, th], [th, edge+th]/*,[th, zmax]*/]);
}