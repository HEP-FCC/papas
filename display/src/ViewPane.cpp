//
//  Created by Alice Robson on 09/11/15.
//
//
#include "ViewPane.h"

namespace papas {

// Static counter
int ViewPane::nviews = 0;

std::vector<std::string> ViewPane::ProjectionStrings{"xy", "yz", "xz", "ECAL_thetaphi", "HCAL_thetaphi"};

ViewPane::ViewPane(Projection p, int nx, double xmin, double xmax, int ny, double ymin, double ymax, int dx, int dy)
    : m_canvas(ProjectionStrings[p].c_str(), ProjectionStrings[p].c_str(), 50. + ViewPane::nviews * (dx + 10.), 50., dx,
               dy),
      m_projection(p) {

  TH1::AddDirectory(false);
  m_hist = TH2F(ProjectionStrings[p].c_str(), ProjectionStrings[p].c_str(), nx, xmin, xmax, ny, ymin, ymax);
  TH1::AddDirectory(true);
  m_hist.Draw();
  m_hist.SetStats(false);
  ViewPane::nviews += 1;
}

/**
 * \param[in]  obj  Shared pointer to a Drawable object
 * \param[in]  layer Which layer to plot at (integer)
 * \param[in]  clearable Whether drawable objects are clearable vs locked. Default to true (clearable)
 */
void ViewPane::addToRegister(std::shared_ptr<Drawable> obj, int layer, bool clearable) {
  // TODO think if shared_ptr is best way
  m_registered.push_back(std::pair<std::shared_ptr<Drawable>, int>{obj, layer});

  if (!clearable) {  // these are the things we always want to see
    m_locked.push_back(std::pair<std::shared_ptr<Drawable>, int>{obj, layer});
  }
}

void ViewPane::clear()  // clear everything except locked items
{
  m_registered = m_locked;
}

void ViewPane::draw() {
  m_canvas.cd();

  // this sorts m_registered by layer (its second integer argument)
  // NB for this needed to avoid use of map which cannot be sorted on its second element
  std::sort(m_registered.begin(), m_registered.end(),
            [](const std::pair<std::shared_ptr<Drawable>, int>& left,
               const std::pair<std::shared_ptr<Drawable>, int>& right) { return left.second < right.second; });
  // std::cout << "vector " << m_registered.size();

  // Now draw all registered items
  for (auto const& reg : m_registered) {
    reg.first->Draw(ProjectionStrings[m_projection]);
  }

  // Flush to screen
  m_canvas.Update();
  ;
}
  void ViewPane::saveAs(const char* fname) {
    m_canvas.SaveAs(fname);
  }

void ViewPane::zoom(double xmin, double xmax, double ymin, double ymax) {
  m_hist.GetXaxis()->SetRangeUser(xmin, xmax);
  m_hist.GetYaxis()->SetRangeUser(ymin, ymax);
  m_canvas.Update();
}

void ViewPane::unZoom() {
  m_hist.GetXaxis()->UnZoom();
  m_hist.GetYaxis()->UnZoom();
  m_canvas.Modified();
  m_canvas.Update();
}

}  // end namespace papas
