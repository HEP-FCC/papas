
from geotools import circle_intersection


class Info(object):
    pass

class Propagator {
public:  
    Propogator(const Detector&);
    virtual void PropagateOne(Particle, std::string Layer, bool inner =true)=0;
protected:
    const Detector& m_detector;
                
};

class StraightLinePropagator : public Propagator {
public:  
    using Propogator(const Detector&);
    void PropagateOne(Particle, std::string Layer, bool inner =true) override;
private:
    
                
};

class HelixPropagator : public Propagator {}
public:  
    using Propogator(const Detector&, double field);
    void PropagateOne(Particle, std::string Layer, bool inner =true) override;
private:
    double m_field;
                
};

class HelixPropagator(Detector detector) :       
    Propagator(detector)


    def propagate_one(self, particle, cylinder, dummy=None):
        line = StraightLine(particle.p4(), particle.vertex) 
        print particle.p3().X(),particle.p3().Y(),particle.p3().Z()
        print particle.vertex.X(),particle.vertex.Y(),particle.vertex.Z()
        
        particle.set_path( line )
        theta = line.udir.Theta()
        if line.udir.Z():
            destz = cylinder.z if line.udir.Z() > 0. else -cylinder.z
            length = (destz - line.origin.Z())/math.cos(theta)
            # import pdb; pdb.set_trace()
            assert(length>=0)
            destination = line.origin + line.udir * length
            rdest = destination.Perp()
            if rdest > cylinder.rad:
                udirxy = TVector3(line.udir.X(), line.udir.Y(), 0.)
                originxy = TVector3(line.origin.X(), line.origin.Y(), 0.)
                # solve 2nd degree equation for intersection
                # between the straight line and the cylinder
                # in the xy plane to get k,
                # the propagation length
                a = udirxy.Mag2()
                b= 2*udirxy.Dot(originxy)
                c= originxy.Mag2()-cylinder.rad**2
                delta = b**2 - 4*a*c
                km = (-b - math.sqrt(delta))/(2*a)
                # positive propagation -> correct solution.
                kp = (-b + math.sqrt(delta))/(2*a)
                # print delta, km, kp
                destination = line.origin + line.udir * kp  
        #TODO deal with Z == 0 
        #TODO deal with overlapping cylinders
        print destination.X(),destination.Y(),destination.Z()
        particle.points[cylinder.name] = destination
        print "striaght point"  + cylinder.name

        
class HelixPropagator(Propagator):
    
    def propagate_one(self, particle, cylinder, field, debug_info=None):
        helix = Helix(field, particle.q(), particle.p4(),
                      particle.vertex)
        particle.set_path(helix)
        is_looper = helix.extreme_point_xy.Mag() < cylinder.rad
        is_positive = particle.p4().Z() > 0.
        if not is_looper:
            xm, ym, xp, yp = circle_intersection(helix.center_xy.X(),
                                                 helix.center_xy.Y(),
                                                 helix.rho,
                                                 cylinder.rad)
            # particle.points[cylinder.name+'_m'] = Point(xm,ym,0)
            # particle.points[cylinder.name+'_p'] = Point(xp,yp,0)
            phi_m = helix.phi(xm, ym)
            phi_p = helix.phi(xp, yp)
            dest_time = helix.time_at_phi(phi_p)
            destination = helix.point_at_time(dest_time)
            if destination.Z()*helix.udir.Z()<0.:
                dest_time = helix.time_at_phi(phi_m)
                destination = helix.point_at_time(dest_time)
            if abs(destination.Z())<cylinder.z:
                particle.points[cylinder.name] = destination
                print  str(particle.pdgid) + " : helix point "  + cylinder.name + str( destination.Z())
            else:
                is_looper = True
        if is_looper:
            # extrapolating to endcap
            destz = cylinder.z if helix.udir.Z() > 0. else -cylinder.z
            dest_time = helix.time_at_z(destz)
            destination = helix.point_at_time(dest_time)
            # destz = cylinder.z if positive else -cylinder.z
            particle.points[cylinder.name] = destination
            print "helix point"  + cylinder.name
            
        info = Info()
        info.is_positive = is_positive
        info.is_looper = is_looper
        return info
        
straight_line = StraightLinePropagator()

helix = HelixPropagator() 
