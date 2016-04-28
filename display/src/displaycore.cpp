//
//  Created by Alice Robson on 09/11/15.
//
//
#include "displaycore.h"
#include <iostream> //temp
#include <cmath>
#include "displaygeometry.h"
#include "displaypfobjects.h"

//Static counter
int ViewPane::nviews = 0;

Display::Display(std::list<Projection> views)
{
   //TODO consider making views concrete objects
   if (views.size() == 0) {
      views = {Projection::xy, Projection::yz, Projection::xz};
   }
   ///Creates viewpanes //AJRTODO use an ENUM instead
   for (auto view : views) {
      if (view == Projection::xy | view == Projection::yz | view == Projection::xz) {
         m_views[to_str(view)] = std::unique_ptr<ViewPane> {
            new ViewPane(view,
            100, -4, 4, 100, -4, 4)
         };
      } else if (view == Projection::ECAL_thetaphi
                 || view == Projection::HCAL_thetaphi) { //AJRTODO check this
         m_views[to_str(view)] = std::unique_ptr<ViewPane> {
            new ViewPane(view,
            100, -M_PI / 2., M_PI / 2.,
            100, -M_PI, M_PI,
            500, 1000)
         };
      }
   }

};

void Display::addToRegister(std::shared_ptr<Drawable>  obj, int layer,
                       bool clearable)
{
   for (auto const& view : m_views) {
      view.second->addToRegister(obj, layer, clearable);
   }

   /*elems = [obj];
    if hasattr(obj, '__iter__'):
    elems = obj
    for elem in elems:
    for view in m_views.values():
    view.addToRegister(elem, layer, clearable)*/
};


void Display::clear()
{
   for (auto const& view  : m_views) {
      view.second->clear();
   }
}

void Display::unZoom()
{
   for (auto const& view : m_views) {
      view.second->unZoom();
   }
}

void Display::draw() const
{
   for (auto const& view : m_views) {
      view.second->draw();
   }
}



ViewPane::ViewPane(Projection p, int nx,
                   double xmin, double xmax, int ny, double ymin, double ymax,  int dx , int dy) :
   m_canvas(to_str(p).c_str(), to_str(p).c_str(),
            50. + ViewPane::nviews * (dx + 10.), 50.,
            dx, dy),
   m_projection(p)
{

   TH1::AddDirectory(false);
   m_hist = TH2F(to_str(p).c_str(), to_str(p).c_str(), nx, xmin, xmax, ny, ymin,
                 ymax);
   TH1::AddDirectory(true);
   m_hist.Draw();
   m_hist.SetStats(false);
   ViewPane::nviews += 1 ;
}

/**
 * \param[in]  obj  Shared pointer to a Drawable object
 * \param[in]  layer Which layer to plot at (integer)
 * \param[in]  clearable Whether drawable objects are clearable vs locked. Default to true (clearable)
 */
void ViewPane::addToRegister(std::shared_ptr<Drawable> obj, int layer,
                        bool clearable)
{
   //TODO think if shared_ptr is best way
   m_registered.push_back(std::pair<std::shared_ptr<Drawable> , int> { obj, layer});

   if (!clearable) { // these are the things we always want to see
      m_locked.push_back(std::pair<std::shared_ptr<Drawable> , int> {obj, layer});
   }
}

void ViewPane::clear() // clear everything except locked items
{
   m_registered = m_locked;
}

void ViewPane::draw()
{
   m_canvas.cd();

   // this sorts m_registered by layer (its second integer argument)
   // NB for this needed to avoid use of map which cannot be sorted on its second element
   std::sort(m_registered.begin(), m_registered.end(),
             [](const std::pair<std::shared_ptr<Drawable>, int>& left,
   const std::pair<std::shared_ptr<Drawable>, int>& right) {
      return left.second < right.second;
   });
   //std::cout << "vector " << m_registered.size();

   //Now draw all registered items
   for (auto const& reg : m_registered) {
      reg.first->Draw(to_str(m_projection));
   }

   //Flush to screen
   m_canvas.Update();
   ;

}

void ViewPane::zoom(double xmin, double xmax, double ymin, double  ymax)
{
   m_hist.GetXaxis()->SetRangeUser(xmin, xmax);
   m_hist.GetYaxis()->SetRangeUser(ymin, ymax);
   m_canvas.Update();
}

void ViewPane::unZoom()
{
   m_hist.GetXaxis()->UnZoom();
   m_hist.GetYaxis()->UnZoom();
   m_canvas.Modified();
   m_canvas.Update();
}


/**
 * \param[in]  name  Name to show at top of viewpane
 * \param[in]  projection Which projections (usually same as name)
 * \param[in]  nx Number of points on x axis
 * \param[in]  xmin Minimum value for x axis
 * \param[in]  xmax Maximum value for x axis
 * \param[in]  ny Number of points on y axis
 * \param[in]  ymin Minimum value for y axis
 * \param[in]  ymax Maximum value for y axis
 * \param[in]  dx ?
 * \param[in]  dy ?
 */