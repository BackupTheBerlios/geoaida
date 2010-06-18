/***************************************************************************
                          nodelist.cpp  -  description
                             -------------------
    begin                : Thu Oct 19 2000
    copyright            : (C) 2000 by Jürgen Bückner
    email                : bueckner@tnt.uni-hannover.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

using namespace std;

//  #define DEBUGMSG

#include "nodelist.h"
#include "gaimage.h"
//#include "gaimaget.h"
#include "garegion.h"
//#include "garegiont.h"

using namespace Ga;

QDict<Ga::Image> NodeList::imageDict_;

/** default constructor */
NodeList::NodeList()
{
  qDebug("### NodeList::NodeList()");
  setAutoDelete(FALSE);
#if 0
  p_=0.0;
#endif
  gW_=gN_=gE_=gS_=0.0;
}

/** Copy constructor */
NodeList::NodeList(const NodeList& nl, bool deep) : QDict<Node>(nl)
{
  qDebug("### NodeList::NodeList(NodeList& nl, deep=%d)",deep);
  copy(nl,deep);
}

NodeList& NodeList::operator=(const NodeList& nl)
{
  qDebug("### NodeList::operator=(NodeList& nl)");
  if (this!=&nl) copy(nl,true);
  return *this;
}

void NodeList::copy(const NodeList& nl, bool deep)
{
qDebug("### NodeList::copy(NodeList& nl, deep=%d)",deep);
  QDict<Node>::operator=(nl);
  if (deep) {
    list_=nl.list_;
    maxID_=nl.maxID_;
    stack_=nl.stack_;
  }
#if 0
  p_=nl.judgement();
#endif
  attribList_=nl.attribList_;
  gW_=nl.gW_;
  gN_=nl.gN_;
  gE_=nl.gE_;
  gS_=nl.gS_;
  xRes_=nl.xRes_;
  yRes_=nl.yRes_;
  sizeX_=nl.sizeX_;
  sizeY_=nl.sizeY_;
  maxID_=nl.maxID_;
}

/** constructor resd from the provided filename*/
NodeList::NodeList(QString filename) {
  read(filename);
#if 0
  p_=0.0;
#endif
}

/** denstructor */
NodeList::~NodeList()
{
}

/** read a list of Node and the attributes from the provided filename */
void NodeList::read(QString filename) {
  QFile fp(filename);
  if (!fp.open(IO_ReadOnly)) {
    qDebug("NodeList::read(%s): file not found\n",(const char*)filename);
    return;
  }
	read(fp);
	fp.close();
}

/** read a list of Node and the attributes from the provided filepointer */
void NodeList::read(QIODevice& fp) {
  MLParser parser(&fp);
  read(parser);
}

/** set nodelist attribute (group attribute) */
void NodeList::set(QString name, QString val)
{
  attribList_.replace(name,val);
}

/** get nodelist attribute (group attribute) */
QString NodeList::get(QString name)
{
  QString *val=attribList_[name];
  if (val) return *val;
  else {
    cerr << "Warning: Attribute not set in nodelist: " << name << endl;
    return "";
  }
}

/** calculate new geo coordinates and resolution using all images belong to included nodes*/
void NodeList::calcNewGEOValues(float &gN,float &gS,float &gW,float &gE,
				int &sizeX,int &sizeY,float &xRes,float &yRes) {
  sizeX = sizeY = 0;
  gN = gS = gW = gE = 0.0;
  xRes=0;
  yRes=0;
  if (isEmpty()) return;
  Node* node;
  QStringList::Iterator it = list_.begin();
  node = find(*it);
  if (node) { //initialize variables
    gN=node->geoNorth();
    gS=node->geoSouth();
    gW=node->geoWest();
    gE=node->geoEast();
    yRes = (gN-gS) / float(node->sizeY()); //(node->lly() - node->ury()); //resolution
    xRes = (gE-gW) / float(node->sizeX()); //(node->urx() - node->llx()); //resolution
  }
  for ( it = list_.begin(); it != list_.end(); ++it ) {
    node = find(*it);
    if (gN < node->geoNorth()) gN = node->geoNorth();// calculate coordinates of the corner points
    if (gS > node->geoSouth()) gS = node->geoSouth();
    if (gW > node->geoWest()) gW = node->geoWest();
    if (gE < node->geoEast()) gE = node->geoEast();
    float xr = (node->geoEast()-node->geoWest()) / float(node->sizeX());//calculate resolution
    float yr = (node->geoNorth()-node->geoSouth()) / float(node->sizeY());
    if (xr < xRes) xRes = xr;
    if (yr < yRes) yRes = yr;
    qDebug("#+#+#geoNorth: %f,geoSouth: %f,geoWest: %f, geoEast: %f, rows: %d, cols: %d, lly: %d, ury: %d, llx: %d, urx: %d, dy: %f, dx: %f (%f,%f)",
           node->geoNorth(),node->geoSouth(),node->geoWest(),node->geoEast(),
           node->sizeX(),node->sizeY(), node->lly(), node->ury(), node->llx(), node->urx(),
           yr, xr,xRes,yRes);
  }
  if (xRes==0.0 || yRes==0.0) return;
  sizeX = int((gE-gW) / xRes+0.5);//calculate new image size
  sizeY = int((gN-gS) / yRes+0.5);//calculate new image size
  qDebug("#+#+#newX: %d, newY: %d, xRes: %f, yRes: %f, gN: %f, gS: %f, gE: %f, gW: %f",
	 sizeX, sizeY, xRes, yRes, gN, gS, gE, gW);
}

/** read a list of Node and the attributes through parser */
void NodeList::read(MLParser& parser) {
  qDebug("NodeList::read(parser)");
  list_.clear();
  QString keywords[]={"node","region",""};
  const MLTagTable nodeTagTable(keywords);
  const int TOK_NODE=1;
  const int TOK_REGION=2;
  const int TOK_ARG_LIST=3;
  int tag;
  do {
    tag=parser.tag(nodeTagTable);
    switch (tag) {
    case TOK_NODE:
    case TOK_REGION: {
      Node* gi=new Node(parser); //parse node
      if (gi->filename()=="") {
	qDebug("NodeList::read() Missing filename");
	delete gi;
	break;
      }
      this->insert(gi->key(),gi); //insert read node
      qDebug("####### %s (%s)",gi->key().latin1(),gi->name().latin1());
      break;
    }
    default: {
      ArgDict* args=parser.args();
      delete args;
      break;
    }
    }
  } while ((tag!=MLParser::END_OF_FILE ) && (tag != -TOK_ARG_LIST));
  
  qDebug("NodeList::read: num nodes=%d",count());
  if (isEmpty()) return;
  //test images and create a structure for the images
  QString key;
  for ( QStringList::Iterator it = list_.begin(); it != list_.end(); ++it ) {
    key = *it;
    Node *img = find(key);
    //qDebug("## NodeList::read(parser)" + img->filename());
    img->load(*this);
  }
  calcNewGEOValues(gN_,gS_,gW_,gE_,sizeX_, sizeY_, xRes_,yRes_);
  qDebug("$$read gN_:%f gS_:%f gW_:%f gE_:%f sizeX_:%d sizeY_:%d xRes_:%f yRes_:%f ",gN_,gS_,gW_,gE_,sizeX_, sizeY_, xRes_,yRes_);
  
  outImage_=Image(typeid(int),sizeX_,sizeY_,1);
  outImage_.setGeoCoordinates(gW_, gN_, gE_, gS_);
}

/** operator +=  for NodeList*/
NodeList& NodeList::operator += (NodeList& nl){
	QDictIterator<Node> it( nl );
	while ( it.current() ) {
		this->insert(it.currentKey().latin1(),it.current());
		++it;
	}
	return *this;
}

/** return void pointer to the Node class */
Node* NodeList::node(QString imgKey) {
  return find(imgKey);
}
/** return void pointer to the image data */
void* NodeList::data(QString imgKey) {
  return (find(imgKey))->data();
}
/** return the cols numbers of the image */
int NodeList::cols(QString imgKey) {
  return (find(imgKey))->cols();
}
/** return the rows numbers of the image */
int NodeList::rows(QString imgKey) {
  return (find(imgKey))->rows();
}
/** return geoNorth value of the image*/
float NodeList::geoNorth(QString imgKey) {
  return (find(imgKey))->geoNorth();
}
/** return geoSouth value of the image*/
float NodeList::geoSouth(QString imgKey) {
  return (find(imgKey))->geoSouth();
}
/** return geoEast value of the image*/
float NodeList::geoEast(QString imgKey) {
  return (find(imgKey))->geoEast();
}
/** return geoWest  valueof the image*/
float NodeList::geoWest(QString imgKey) {
  return (find(imgKey))->geoWest();
}
/** return Info the image*/
void NodeList::info(QString imgKey) {
  (find(imgKey))->info();
}

#if 0
//** write a scrap of the data to a file*/
QString NodeList::part(QString imgKey, float north, float south, float west, float east,
  QString fname) {
  if (find(imgKey))
    return (find(imgKey))->part(north, south, west, east, fname);
  else
    qWarning("## (Warning) NodeList::part(%s, %f, %f, %f, %f) no entry",
          (const char*)imgKey, north, south, west, east);
}
#endif

/** print info  */
void  NodeList::info(void){
  for ( QStringList::Iterator it = list_.begin(); it != list_.end(); ++it ) {
    Node *node = find(*it);
		node->info();
		printf("\n");
  }

}

void NodeList::insert(QString key, Node* node){
	QDict<Node>::replace( key, node );//insert node
//	QDict<Node>::insert( key, node );//insert node
	list_ += key; //fill additional list of image names
}

/** return a list of pointer to the objects of specified class type */
NodeList* NodeList::selectClass (QString classname){
  //xNodeList* nl = new NodeList(*this, false);
  NodeList* nl = new NodeList;
  Node* node;
  for ( QStringList::Iterator it = list_.begin(); it != list_.end(); ++it ) {
    node = find(*it);
    if (!node) {
      qDebug("NodeList::selectClass: %s Warning: %s not found in nodelist",
	     classname.latin1(),(*it).latin1());
      continue;
    }
    if (classname.compare(node->classname()) == 0) {
      nl->insert (node->key(), node );//insert node
    }
  }
  return nl;
}

/** calculate for all classes 'classname' the term 'term' with the function 'fkt'.
		Fkt. return int*/
void NodeList::calcForSelect (QString classname, QString term, int (*fkt)(void)) {
  Node* node;
  //NodeList* nl = selectClass(classname);
  for ( QStringList::Iterator it = list_.begin(); it != list_.end(); ++it ) {
    node = find(*it);
    if (!node) {
      qDebug("NodeList::calcForSelect: node %s not found",(*it).latin1());
      continue;
    }
    if (classname.compare(node->classname()) == 0) {
      QString *val = new QString;
      val->setNum(fkt());
      if (!find(term)) node->insert(term, val);
      else qWarning("##. (Warning) NodeList::calcForSelect: %s - exist",term.latin1());
    } //else qDebug("## %s ##",node->classname().latin1());
  }
}

/** calculate for all classes 'classname' the term 'term' with the function 'fkt'.*/
void NodeList::calcForSelect (QString classname, QString term, QString (*fkt)(void)) {
  Node* node;
  for ( QStringList::Iterator it = list_.begin(); it != list_.end(); ++it ) {
    node = find(*it);
    if (!node) {
      qDebug("NodeList::calcForSelect: node %s not found",(*it).latin1());
      continue;
    }
    if (classname.compare(node->classname()) == 0) {
      QString *val = new QString;
      *val=fkt();
      if (!find(term)) node->insert(term, val);
      else qDebug("## (Warning) NodeList::calcForSelect: %s - exist",term.latin1());
    } //else qDebug("## .%s. ##",node->classname().latin1());
  }
}

/** calculate for all classes 'classname' the term 'term' with the fun*/
void NodeList::calcForSelect (QString classname, QString term, GASensor* sensor) {
  Node* node;
  for ( QStringList::Iterator it = list_.begin(); it != list_.end(); ++it ) {
    node = find(*it);
    if (!node) {
      qDebug("NodeList::calcForSelect: node %s not found",(*it).latin1());
      continue;
    }
    if (classname.compare(node->classname())==0 || classname.compare("ALL")==0) {
      Image* img=node->data();
      if (!img) {
	cerr << "NodeList::calcForSelect: node data not read" << endl;
	exit(1);
      }
      sensor->image(*img);
#if 0		
      sensor->readImage(*((*node)["file"])); //["labelpic"]));
#endif
      sensor->node(node);
      sensor->calc(term);
    } else qDebug("## .%s. ##",node->classname().latin1());
  }
}

/** calculate for all classes 'classname' the term 'term' with the fun*/
void NodeList::calcForSelect (QString term, GASensor* sensor) {
  Node* node;
  for ( QStringList::Iterator it = list_.begin(); it != list_.end(); ++it ) {
    node = find(*it);
    if (!node) {
      qDebug("NodeList::calcForSelect: node %s not found",(*it).latin1());
      continue;
    }
    //if (classname.compare(node->classname())==0 || classname.compare("ALL")==0) {
    Image* img=node->data();
    if (!img) {
      cerr << "NodeList::calcForSelect: node data not read" << endl;
      exit(1);
    }
    sensor->image(*img);
#if 0
    sensor->readImage(*((*node)["file"])); //["labelpic"]));
#endif
    sensor->node(node);
    sensor->calc(term);
    //} else qDebug("## .%s. ##",node->classname().latin1());
  }
}

/** merge for all images in list (scale to a common resulution)
* write result to file 'fname'
  flag 'newReg'
    true - generate new nodes when regions are split into subregions
    false - one node can associate to more than on region
*/
//NodeList* NodeList::merge (QString outImgName, bool newReg = 0) {
NodeList* NodeList::merge (bool newReg, QString outImgName) {
  //calc new geo coordinates using all nodes of liste 'this'
  if (gN_==0.0 || gS_==0.0) calcNewGEOValues(gN_,gS_,gW_,gE_,sizeX_, sizeY_, xRes_,yRes_);
  qDebug("$#$merge gN_:%f gS_:%f gW_:%f gE_:%f sizeX_:%d sizeY_:%d xRes_:%f yRes_:%f ",gN_,gS_,gW_,gE_,sizeX_,sizeY_,xRes_,yRes_);
  

  /*
    Merging is only necessary, when there might be a confict between regions.
    If all regions share the same label image this cannot be the case.
    
    Implemented 2009-05-19, Christian Becker
   */

  {
    QDictIterator<Node> it(*this);
    bool merging_necessary=false;
    {
      it.toFirst();
      if (it.current()){      
        QString oldFileName = it.current()->filename();
      
	while(it.current()) {
	  Node *node=it.current();
	  if (oldFileName != node->filename()){
	    merging_necessary=true;
	    break;
	  }
	  ++it;
	}
      }
    }

    if (!merging_necessary){

      // There is nothing to merge    
      
      it.toFirst();
      Node *node=it.current();      
      if (it.current()){      
	outImage_=readLabelFile(node->filename(),node->geoWest(), node->geoNorth(), node->geoEast(), node->geoSouth());      
	NodeList* selected=new NodeList(*this, true);// copy node list
	while(it.current()) {
	  Node *node=it.current();      		  
	  node->filename(outImgName);
	  ++it;
	}
	registerLabelFile(outImgName, outImage_);
	return selected;
      }
    }
  }

  //generate a sorted list of nodes using 'p' the weighing
  SortPtrList sortlist;
  QDictIterator<Node> it( *this ); //iterator for nodelist
  it.toFirst();
  while(it.current()) {
    Node *node = it.current();
    //float ff = ((*node)["p"])->toFloat();
    //SortEle* e = new SortEle(it.current(),(it.current())->p(),it.currentKey());
    SortEle* e;
    if (node->find( "p" ))
      e = new SortEle(it.current(),((*node)["p"])->toFloat(),it.currentKey());
    else
      e = new SortEle(it.current(),(it.current())->p(),it.currentKey());
    sortlist.append(e);
    ++it;
  }
  sortlist.sort();
  
  //merge and relabel the images of all nodes in nodelist
  int freeID=maxID_;
  maxID_++; //label count -> all label are relabeled / label 1 is reserved
  SortEle *se;
  int nllx=0, nlly=0, nurx=0, nury=0;
  int changeVec[sortlist.count()+maxID_]; //vektor, um IDs der veränderten regionen zu erfassen
  for (unsigned int j=0; j<sortlist.count()+maxID_; j++)changeVec[j]=0;//initialisierung
  qDebug("NodeList::merge: changeVec-size %d",  sortlist.count()+maxID_);

  for ( se=sortlist.first(); se != 0; se=sortlist.next() ) {
    Node *node=se->node();
    qDebug("### merge: p=%f, l_id=%d, n_id=%d, read:%s",se->p(), node->id(),maxID_, node->filename().latin1());
    Image& img = readLabelFile(node->filename(),node->geoWest(), node->geoNorth(), node->geoEast(), node->geoSouth());
    if (img.isEmpty()) continue;
    qDebug("NodeList::merge: geoMerge: %4d %4d %4d %4d",
	   node->llx(),node->lly(),node->urx(),node->ury());
    
    outImage_.geoMerge(img, node->id(), maxID_,
                       node->llx(),node->lly(),node->urx(),node->ury(),
                       &nllx, &nlly, &nurx, &nury,
                       (int*)(&changeVec));//merge to out image
    qDebug("Done %4d %4d %4d %4d",
	   nllx,nlly,nurx,nury);
    node->id(maxID_);
    se->setGeoPos(nllx, nlly, nurx, nury);
    maxID_++;
  } //variable maxID is used later ([2..maxID-1] = count_of_old_regions)
  

  /*
    Altered 2009-05-19 Christian Becker
    It shouldn't be necessary to actually write the image
    to hard disk.
    Making it availabe at this point is necessary, though 
    (ie. in node->load(..))    
   */
//  outImage_.write(outImgName);
  registerLabelFile(outImgName, outImage_);

  //calculate new Node attibutes and look for divided regions
  NodeList* selected=new NodeList(*this, false);// new nodelist for result, no deep copy
  //  int *pic=(int*)outImage_.begin();
  for ( se=sortlist.first(); se != 0; se=sortlist.next() ) {
    Node *node=se->node();
    int oldID = node->id(); //urspruengliche ID der region - bleibt in der Schleife immer gleich
    QString addr = node->addr(); //daten zum uebertragen speichern
    QString classname = node->classname();
    QString name = node->name();
    float p =node->p();
    if (node->find( "p" )) p=((*node)["p"])->toFloat();
    //node->clear(); //daten loeschen , !! Removed by Becker 2008-09-28
    node->classname(classname);//neue daten eintragen
    node->filename(outImgName);//node->setValue("file",outImgName);
    node->setValue("addr",addr);
    node->setValue("name",name);
    qDebug("### merge: GEO id=%d; gn=%f, gs=%f, ge=%f, gw=%f, p=%f",node->id(),gN_,gS_,gE_,gW_,p);
    node->replace("p",p);
    node->replace("id",node->id());
    node->replace("file_geoNorth",gN_);
    node->replace("file_geoSouth",gS_);
    node->replace("file_geoEast", gE_);
    node->replace("file_geoWest", gW_);
    node->load(*this);
    node->update();
//node->info();
    if(changeVec)
      if(changeVec[oldID]){ //veraendert?
        node->setValue("change",(new QString)->sprintf("true"));
        if(!newReg) selected->insert(node->key(),node);			
        else {
          for (int y=se->ury; y<=se->lly; y++)
            for (int x=se->llx; x<=se->urx; x++) {
              if (outImage_.getInt(x,y)==oldID) {
                if(oldID != freeID) {
                  int newID=freeID;
                  label4(outImage_, y*outImage_.sizeX()+x, oldID,  newID);
//                  label4(pic, outImage_.sizeX(), outImage_.sizeY(), y*outImage_.sizeX()+x, oldID,  newID);
                  node->id(newID);
                  freeID=oldID;
                  selected->insert(node->key(),node);

                } else {//es sind neue regionen entstanden
                  label4(outImage_, y*outImage_.sizeX()+x, oldID,  maxID_);
                  Node* newNode = new Node; //neuer Knoten
                  newNode->id(maxID_); //neue daten eintragen
                  newNode->classname(classname);
                  //newNode->replace("file",outImgName);
                  node->filename(outImgName);//node->setValue("file",outImgName);
                  newNode->replace("status","HI");
                  newNode->replace("name",name);
                  node->replace("id",node->id());
                  newNode->replace("file_geoNorth",gN_);
                  newNode->replace("file_geoSouth",gS_);
                  newNode->replace("file_geoEast", gE_);
                  newNode->replace("file_geoWest", gW_);
                  newNode->replace("addr",QString().sprintf("%s#%d",(const char*)newNode->name(),maxID_));
                  newNode->replace("change","true");
                  newNode->load(*this);
                  //newNode->info();
                  node->update();
                  selected->insert((new QString)->sprintf("%d",maxID_),newNode);
                  maxID_++;
                }
              }
            }
        }
      }
      else selected->insert(node->key(),node);//unveraenderte einfuegen
  }

//  qDebug("maxId=%d",maxID_);
  Image bb = calcBoundingBoxes(outImage_, maxID_);
  for ( se=sortlist.first(); se != 0; se=sortlist.next() ) {//set bounding box values
    Node *node=se->node();

    node->replace("llx",bb.getInt(node->id(),0));
    node->replace("lly",bb.getInt(node->id(),1));
    node->replace("urx",bb.getInt(node->id(),2));
    node->replace("ury",bb.getInt(node->id(),3));

    // XXX Was ist mit voellig ueberdeckten regionen ??
    if( bb.getInt(node->id(),0)==0 && bb.getInt(node->id(),1)==0 &&
        bb.getInt(node->id(),2)==0 && bb.getInt(node->id(),3)==0 )
      selected->remove(node->key());
    node->update();
  }
  

  return selected;
}

/** write a nodelist to the given textstream */
void NodeList::write(QTextStream &fp, QString groupFileName)
{
  QDictIterator<Node> it(*this);
  if (it.current()) {
    fp << "<group ";
    if (!groupFileName.isEmpty())
      fp << "id=\"" << groupId_  << "\" file=\"" << groupFileName << "\" ";
    fp << attribList_;
    fp.precision(8);
    fp << "file_geoNorth=\"" <<gN_<< "\"file_geoWest=\"" <<gW_<< "\"file_geoEast=\"" <<gE_<< "\"file_geoSouth=\"" <<gS_<< "\"";
    fp << " >" << endl;
    for (;it.current();++it) {
      (*it)->write(fp);
    }
    
    fp << "</group>" << endl;
  }
}

/** write a nodelist as a regionsfile */
void NodeList::writeRegionFile(QTextStream &fp)
{
  QDictIterator<Node> it(*this);
  if (it.current()) {
    for (;it.current();++it) {
#ifdef WIN32
      (*it).write(fp,"region");
#else
   (*it)->write(fp,"region");
#endif
    }
  }

}

Image& NodeList::readLabelFile(QString filename, double gW, double gN, double gE, double gS)
{
  Image *im = imageDict_[filename];
  if (im)
    return *im;                  // image found in dictionary

  qDebug("###### read new\t%s\n",filename.latin1());
  im = new Image(typeid(int));
  im->read(filename.latin1()); //DAS GEHT HIER NICHT!!!!!!!!!!!!!!!!!!!!!!!!
  if (!im->isEmpty()) {//ODER DAS !!!!!!!!!!!!!!!!!!!!!!!!
    im->setGeoCoordinates(gW, gN, gE, gS);
    imageDict_.replace(filename, im);
  }

  return *im;
}

void NodeList::registerLabelFile(QString filename, const Image& labelimage){
    imageDict_.replace(filename, &labelimage);
}

/** write the resulting label image */
void NodeList::writeOutImage(QString filename)
{
  if (outImage_.isEmpty()) return;
  outImage_.write(filename);
}

/** write the resulting group label image */
void NodeList::writeGroupImage(QString filename)
{
  if (groupImage_.isEmpty()) return;
  groupImage_.write(filename);
}

void NodeList::genGroupImage()
{
  if (!groupIdCounter_) {
      groupImage_=Image(typeid(int),outImage_.sizeX(),outImage_.sizeY());
  }
  groupId_=++groupIdCounter_;
  QDictIterator<Node> it( *this );

  for (;it.current();++it) {
    Node* node=it.current();
    int id=node->id();
    int llx=node->llx();
    int urx=node->urx();
    if (urx >= outImage_.sizeX()) 
	urx=outImage_.sizeX()-1;
    int lly=node->lly();
    if (lly >= outImage_.sizeY()) 
	lly=outImage_.sizeY()-1;
    int ury=node->ury();

    for (int y=ury; y<=lly; y++) {
      int *outImagePtr= (int *) outImage_.begin(y,0);
      int *groupImagePtr= (int*) groupImage_.begin(y,0);
      outImagePtr+=llx;
      groupImagePtr+=llx;
      for (int x=llx; x<=urx; x++) {
	int v=*outImagePtr;
        if (v==id) {
          *groupImagePtr = groupIdCounter_;
	}
	outImagePtr++;
	groupImagePtr++;
      } 
    }
    
  }
}


/** return stack - for bottom-up */
Stack& NodeList::stack(void){
  return stack_;
}
/** Removes the top item from the local stack and returns it. */
StackElem* NodeList::stackPop(){
 return stack_.pop();
}
/** Adds an element d to the top of the local stack. */
void NodeList::stackPush(const StackElem* d){
  stack_.push(d);
}
/** Returns the number of items in the local stack.  */
uint NodeList::stackCount(void){
  return stack_.count();
}
/** Removes all items from the local stack, deleting them if autoDelete() is TRUE.  */
void NodeList::stackClear(void){
  stack_.clear();
}
/** Removes the top item from the local stack and deletes it if autoDelete() is TRUE. Returns TRUE if there was an item to pop;
otherwise returns FALSE.  */
bool NodeList::stackRemove (void) {
  return stack_.remove();
}
/** Returns TRUE is the local stack contains no elements to be popped; otherwise returns FALSE.  */
bool NodeList::stackIsEmpty (){
  return stack_.isEmpty();
}

int NodeList::maxID_=1;
int NodeList::groupIdCounter_=0;
Image NodeList::outImage_;
Image NodeList::groupImage_;
/** copy the nodes in this nodelist into the group image */

/** set the out-image-file-name in all nodes of NODELIST */
void NodeList::setImgName(QString outImgName){
  QDictIterator<Node> it( *this ); //iterator for nodelist
  it.toFirst();
  while(it.current()) {
    Node *node = it.current();
#ifdef WIN32
    node->replace("file",&outImgName);
#else
    node->replace("file",outImgName);
#endif
    ++it;
  }
}

