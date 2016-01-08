//
//  Created by Alice Robson on 09/11/15.
//
//
#include <iostream> //temp
#include "displaycore.h"
#include "displaygeometry.h"
#include "displaypfobjects.h"

#include <cmath>




Display::Display(std::list<enumProjection> views)
{
   if (views.size() == 0) {
      views = {enumProjection::xy, enumProjection::yz, enumProjection::xz};
   }
   ///Creates viewpanes //AJRTODO think about using an ENUM instead
   for (auto view : views) {
      //std::cout<< view << " " << view.find("thetaphi") << " and  " << view.npos <<std::endl;
      if (view == enumProjection::xy | view == enumProjection::yz | view == enumProjection::xz) {
         m_views[to_str(view).c_str()] = std::unique_ptr<ViewPane> {new ViewPane(view,
                  100, -4, 4, 100, -4, 4)
         };
      }
      else if (view == enumProjection::ECAL_thetaphi|| view == enumProjection::HCAL_thetaphi) { //AJRTODO check this
         m_views[to_str(view).c_str()] = std::unique_ptr<ViewPane> {new ViewPane(view,
                  100, -M_PI / 2., M_PI / 2.,
                  100, -M_PI, M_PI,
                  500, 1000)
         };
      }
   }

};

void Display::Register(std::shared_ptr<Drawable>  obj, int layer,
                       bool clearable)
{
   for (auto const& view : m_views) {
      view.second->Register(obj, layer, clearable);
   }

   /*elems = [obj];
    if hasattr(obj, '__iter__'):
    elems = obj
    for elem in elems:
    for view in m_views.values():
    view.register(elem, layer, clearable)*/
};


void Display::Clear()
{
   for (auto const& view  : m_views) {
      view.second->Clear();
   }
}

void Display::Unzoom()
{
   for (auto const& view : m_views) {
      view.second->UnZoom();
   }
}
void Display::Draw() const
{
   for (auto const& view : m_views) {
      view.second->Draw();
   }
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

ViewPane::ViewPane(enumProjection p, int nx,
                   double xmin, double xmax, int ny, double ymin, double ymax,  int dx , int dy) :
m_canvas( to_str(p).c_str() ,to_str(p).c_str() , 50. + ViewPane::nviews * (dx + 10.), 50.,
            dx, dy), m_projection(p)
{

  
   TH1::AddDirectory(false);
   m_hist = TH2F( to_str(p).c_str() , to_str(p).c_str() , nx, xmin, xmax, ny, ymin, ymax);
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
void ViewPane::Register(std::shared_ptr<Drawable> obj, int layer,
                        bool clearable)
{
   m_registered.push_back(std::pair<std::shared_ptr<Drawable> , int> { obj, layer});
   //std::cout<<"vector "<< m_registered.size();
   if (!clearable) { // these things we always want
      m_locked.push_back(std::pair<std::shared_ptr<Drawable> , int> {obj, layer});
   }
}

void ViewPane::Clear() // clear everything except locked items
{
   m_registered = m_locked;
}

void ViewPane::Draw()
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

void ViewPane::Zoom(double xmin, double xmax, double ymin, double  ymax)
{
   m_hist.GetXaxis()->SetRangeUser(xmin, xmax);
   m_hist.GetYaxis()->SetRangeUser(ymin, ymax);
   m_canvas.Update();
}

void ViewPane::UnZoom()
{
   m_hist.GetXaxis()->UnZoom();
   m_hist.GetYaxis()->UnZoom();
   m_canvas.Modified();
   m_canvas.Update();
}