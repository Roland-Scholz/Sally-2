thick = 2;

xmax = 250;
ymax = 180+2*thick;
zmax = 39;

edge=5;

//xmt = xmax + 2*thick;
//ymt = ymax + 2*thick;    


difference(){
    shell_bottom();
    translate([thick/2,3*thick,zmax-thick-1])
        cube([thick,ymax-6*thick, 3]);
    translate([xmax-thick*1.5,3*thick,zmax-thick-1])
        cube([thick,ymax-6*thick, 3]);
}

module shell_bottom() {
    
    difference(){
        base_shape();
        cubes_left();    
        cubes_right();
        
        //holes for screws
        translate([edge+5,28,0])
            cylinder(3, 2.7, 2.7);
        translate([xmax-edge-5,28,0])
            cylinder(3, 2.7, 2.7);
        translate([edge+5,ymax-24,0])
            cylinder(3, 2.7, 2.7);
        translate([xmax-edge-5,ymax-24,0])
            cylinder(3, 2.7, 2.7);
    }


    
    translate([0, ymax+thick])
        base_shape_holder();
    translate([0, ymax-thick])
        base_shape_holder();
    translate([0, 0])
        base_shape_holder();
    translate([0, 0+2*thick])
        base_shape_holder();    
    
    translate([0, 26, 10])
        cube([8,4,25]);
    translate([0, ymax-26, 10])
        cube([8,4,25]);
    translate([xmax-edge-2*thick+1, 26, 10])
        cube([8,4,25]);
    translate([xmax-edge-2*thick+1, ymax-26, 10])
        cube([8,4,25]);
}


module base_shape() {
    translate([0, ymax+thick, 0])
        rotate([90,0,0]) 
            linear_extrude(ymax+thick)
                base_shape_2d(thick);

    translate([edge+5,28,0])
        screw(4);
    translate([xmax-edge-5,28,0])
        screw(4);
    translate([edge+5,ymax-24,0])
        screw(4);
    translate([xmax-edge-5,ymax-24,0])
        screw(4);

}

module base_shape_holder() {
    
    rotate([90,0,0])
    linear_extrude(thick)
        base_shape_2d(3*thick);

}

module base_shape_2d(th) {
    polygon([[0, zmax], [0, edge], [edge,0], [xmax-edge, 0], [xmax, edge], [xmax, zmax],[xmax-th,zmax], [xmax-th, edge+th], [xmax-edge-th, th],[edge+th, th], [th, edge+th],[th, zmax]]);
}

module cubes_left() {
    for(i = [10:8:ymax-14])
      translate([0, i, 0])
      cube([edge+thick, 4, edge+thick]);
}

module cubes_right() {
    for(i = [10:8:ymax-14])
      translate([xmax-edge-thick, i, 0])
      cube([edge+thick, 4, edge+thick]);
}
    
module screw(r) {    
    difference() {
        cylinder(zmax,r*1.3,r*1.3);
        translate([0,0,-1]);
            cylinder(zmax+2,r/2+0.1,r/2+0.1);
//        cylinder(5,6,6);
    }
}