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
    translate([0,35,0])
        base_shape_a();
}

module ataritext() {
    linear_extrude(0.4)
        mirror([1,0,0])
        text("ATARI ", 16, "SF Atarian              System:style=Regular");

    translate([-45, 0, 0])
    linear_extrude(0.4)
        mirror([1,0,0])
    text("SALLY-2 ", 12, "SF Atarian System:style=Regular");
}

module base_shape_a() {
        rotate([90,0,0]) 
            linear_extrude(20)
                base_shape_2d_a(0.4);
}


module base_shape_2d_a(th) {
    polygon([/* [0, zmax], */ [0, edge], [edge,0], [xmax-edge, 0], [xmax, edge], /*[xmax, zmax],[xmax-th,zmax],*/ [xmax-th, edge+th], [xmax-edge-th, th],[edge+th, th], [th, edge+th]/*,[th, zmax]*/]);
}