/***************************************************************************
                          relabel.cpp  -  description
                             -------------------
    begin                : 30/07/2004
    copyright            : (C) 2004 by Soenke Mueler
    email                : mueller@tnt.uni-hannover.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <relabel.h>

ImageT<int> *relabelOutput (Image &Img, char *labelname, char *descr)
{
  Image hist = Img.calcHistogram(0, 256, 256);
  int max = 255;

  while (hist.getInt (max, 0, 0) == 0 && max > 0)
    max--;

  if (!max) {
    cout << "no texture classes in output segmentation!" << endl;
    exit (-1);
  }
  else
    cout << "identified " << max << " texture classes in segmentation result" << endl;

  ImageT<int> *result_img = new ImageT<int>(Img.sizeX (), Img.sizeY ());
  result_img->typeImage(_PFM_SINT);

  ImageT<int> tmp_img(Img.sizeX(), Img.sizeY());
  tmp_img.typeImage(_PFM_SINT);

  cout << "neue Bilder angelegt" << endl;

  int *dest, *source;
  unsigned char *orig;

  int sumlabel[max + 1];
  memset(sumlabel, 0, sizeof (int) * (max + 1));

  cout << "memset Befehl" << endl;

  // sumlabel stores the number of labels found for each class,
  // labelling should start with 1
  sumlabel[0] = 1;

  for (int i = 1; i <= max; i++) {
    dest = tmp_img.data();
    orig = (unsigned char *) Img.data();

  cout << "Schleife über i" << endl;

    // extract one class and prepare it for labelling
    for (int j = 0; j < Img.sizeImage (); ++j, ++dest, ++orig)
      if (*orig == i)
        *dest = 0;
      else
        *dest = 1;

    // label the current class and store the number of labels
    // label 0 and 1 are background and label border --> throw them away (-2)!
    sumlabel[i] = labelImage(tmp_img) - 2;

    // sum up the labelnumbers for fusion of the different classes
    sumlabel[i] += sumlabel[i - 1];

    // shift the labels, so that they are unique (fusion)
    source = tmp_img.data();
    dest = result_img->data();
    for (int j = 0; j < tmp_img.sizeImage(); ++j, ++dest, ++source) {
      // if the label is not background or border (-2), add it to the result
      if (*source > 1)
        *dest = *source - 2 + sumlabel[i - 1];
    }
    tmp_img.clear();
  }

  cout << "image relabeled" << endl;

  // generate the label description

  ImageT<int> list;
  calcBoundingBoxes(list, *result_img, sumlabel[max]);

  FILE *fp = fopen (descr, "w");
  if (!fp)
  {
    cout << "error opening file " << descr << " for writing." << endl;
    exit (-1);
  }

  // list only the labels with value greater than zero (0 is background)
  for (int i = 1, j = 0; i < sumlabel[max]; ++i)
  {
    if (i >= sumlabel[j + 1])
      j ++;

    fprintf(fp, "<region class=\"class%i\" id=\"%d\" p=\"0.7\" file=\"%s\" llx=\"%d\" lly=\"%d\" urx=\"%d\" ury=\"%d\" />\n",
  				  j+1, i, labelname, list[0][i], list[1][i], list[2][i], list[3][i]);
  }
  cout << "Regionenbeschreibung erzeugt" << endl;
  fclose(fp);

  return (result_img);
}

int combine (Image *in, Image &mask, char* eval_name, char* path)
{
   int classes[CLASSES + 1], c;
   int refclasses[CLASSES + 1];
   int falseclasses[CLASSES + 1];
   float class_level_weight[CLASSES + 1][MAX_LEVEL + 1];

   int sx = in[0].sizeX(), sy = in[0].sizeY();

   // auf volle Groesse resamplen
   for (int l = 0; l < MAX_LEVEL; l ++)
	   in[l] = in[l].resampleNNplus(sx, sy);

   printf("calculating segmentation reliability,\n");

   memset(class_level_weight, 0, sizeof(float) * (CLASSES + 1) * (MAX_LEVEL + 1));

   for (int l = 0; l < MAX_LEVEL; l ++)
     {
       memset(classes, 0, sizeof(int) * (CLASSES + 1));
       memset(refclasses, 0, sizeof(int) * (CLASSES + 1));
       memset(falseclasses, 0, sizeof(int) * (CLASSES + 1));

       for (int y = 0; y < sy; y ++)
	 {
	   for (int x = 0; x < sx; x ++)
	     {
	       c = mask.getInt(x, y, 0);
	       if (c)
		 {
		   refclasses[c] ++;
		   if (in[l].getInt(x, y, 0) == c)
		     classes[c] ++;
		   else
		     falseclasses[in[l].getInt(x, y, 0)] ++;
		 }
	     }
	 }

       for (int j = 1; j < CLASSES + 1; j ++)
	 refclasses[0] += refclasses[j];

       for (int j = 1; j < CLASSES + 1; j ++)
	 {
	   printf("level %d, class %d: %7d of %7d (%3d\%), false: %7d (%3d\%)\n", l, j, classes[j], refclasses[j], refclasses[j] ? classes[j] * 100 / refclasses[j] : 0, falseclasses[j], falseclasses[j] * 100 / (refclasses[0] - refclasses[j]));
	   // calculate the weight: hit percentage - false classification percentage
	   class_level_weight[j][l] = (float)classes[j] / (float)refclasses[j] - (float)falseclasses[j] / (float)(refclasses[0] - refclasses[j]);
	   class_level_weight[j][MAX_LEVEL] += class_level_weight[j][l];
	 }
     }

	//	output of evaluation matrix
   FILE *fp_eval= fopen(eval_name, "w");

   printf("level  c1    c2    c3    c4\n");
   for (int l = 0; l < MAX_LEVEL; l ++)
     {
       printf("%5d: ", l);

       for (int i = 1; i < CLASSES + 1; i ++)
	 {
	   class_level_weight[i][l] /= class_level_weight[i][MAX_LEVEL];
	   printf("%.3f ", class_level_weight[i][l]);
	   fprintf(fp_eval, "%.3f ", class_level_weight[i][l]);
	 }
       printf("\n");
       fprintf(fp_eval, "\n");
     }
   fclose(fp_eval);

 	//	merge output image
   Image out(typeid(unsigned char), sx, sy);

   printf("analyzing images,\n");

   float classes_f[CLASSES + 1];
   float max; int imax;

   for (int y = 0; y < sy; y ++)
   {
     for (int x = 0; x < sx; x ++)
     {
       memset(classes_f, 0, sizeof(float) * (CLASSES + 1));
       for (int l = 0; l < MAX_LEVEL; l ++)
	 classes_f[in[l].getInt(x, y, 0)] += class_level_weight[in[l].getInt(x, y, 0)][l];
       //	 classes_f[in[l].getInt(x, y, 0)] += 1.0;
	
       max = 0;
       imax = 0;

       for (int i = 1; i < (CLASSES + 1); i ++)
       {
	 if (classes_f[i] > max)
	 {
	   max = classes_f[i];
	   imax = i;
	 }
	 else if (classes_f[i] == max)
	   imax = 0;
       }
       out.set(x, y, imax);
     }
     printf("line: %05d of %05d\r", y, sy - 1);
   }
   printf("\n");

   ImageT<unsigned char> label(out.sizeX(), out.sizeY(), 1);

   for (int y = 0; y < out.sizeY(); y ++)
     for (int x = 0; x < out.sizeX(); x ++)
     {
       label.set(x, y, out.getInt(x, y, 0));
     }

   cout << "despeckle, " << endl;

   for (int i = 1; i < CLASSES + 1; i ++)
     despeckle(label, 100, (unsigned char)i, (unsigned char)0);

   ImageT<float> labelf(label.sizeX(), label.sizeY(), 1);

	char name[255];
	float valf;
	sprintf (name, "%s/out.pfm", path);
   printf("writing output image:%s\n", name);

   for (int y = 0; y < label.sizeY(); y ++)
     for (int x = 0; x < label.sizeX(); x ++)
     {
     valf= label.getFloat(x, y, 0);
     if (valf)
	  labelf.set(x, y, valf);
        else
	  labelf.set(x, y, NAN);
     }

   FILE *fp = fopen(name, "w");
   labelf.write(fp);
   fclose(fp);

	return (1);
}

int compose_result (Image *in, char* eval_name, char* regdes_name, char *outimg_name)
{
   float class_level_weight[CLASSES + 1][MAX_LEVEL + 1];

   int sx = in[0].sizeX(), sy = in[0].sizeY();

	//	read evaluation matrix
   FILE *fp_eval= fopen(eval_name, "r");

   for (int l = 0; l < MAX_LEVEL; l ++){
       for (int i = 1; i < CLASSES + 1; i ++){
	 	   fscanf(fp_eval, "%f ", &(class_level_weight[i][l]));
			cout<<class_level_weight[i][l];
			}
	    fscanf(fp_eval, "\n");
 		 cout<<endl;
     }
   fclose(fp_eval);

   // auf volle Groesse resamplen
   for (int l = 1; l < MAX_LEVEL; l ++)
	   in[l] = in[l].resampleNNplus(sx, sy);

 	//	merge output image
   Image out(typeid(unsigned char), sx, sy);

   printf("analyzing images,\n");

   float classes_f[CLASSES + 1];
   float max; int imax;

   for (int y = 0; y < sy; y ++)
   {
     for (int x = 0; x < sx; x ++)
     {
       memset(classes_f, 0, sizeof(float) * (CLASSES + 1));
       for (int l = 0; l < MAX_LEVEL; l ++)
	    classes_f[in[l].getInt(x, y, 0)] += class_level_weight[in[l].getInt(x, y, 0)][l];
       //	 classes_f[in[l].getInt(x, y, 0)] += 1.0;
	
       max = 0;
       imax = 0;

       for (int i = 1; i < (CLASSES + 1); i ++)
       {
	 if (classes_f[i] > max)
	 {
	   max = classes_f[i];
	   imax = i;
	 }
	 else if (classes_f[i] == max)
	   imax = 0;
       }
       out.set(x, y, imax);
     }
     printf("line: %05d of %05d\r", y, sy - 1);
   }
   printf("\n");

   ImageT<unsigned char> label(out.sizeX(), out.sizeY(), 1);

   for (int y = 0; y < out.sizeY(); y ++)
     for (int x = 0; x < out.sizeX(); x ++)
     {
       label.set(x, y, out.getInt(x, y, 0));
     }

   cout << "despeckle, " << endl;
   for (int i = 1; i < CLASSES + 1; i ++)
     despeckle(label, 100, (unsigned char)i, (unsigned char)0);

   ImageT<float> labelf(label.sizeX(), label.sizeY(), 1);
   float valf;

   for (int y = 0; y < label.sizeY(); y ++)
     for (int x = 0; x < label.sizeX(); x ++)
     {
       valf = label.getFloat(x, y, 0);
       if (valf)
	 labelf.set(x, y, valf);
       else
	 labelf.set(x, y, NAN);
     }

	for (int l=0; l<MAX_LEVEL; l++)
		in[l]= Image();
// 	free; Don't know what this is for

#ifdef DEBUG_MSG	
   printf("writing output image.\n");
   FILE *fp = fopen(outimg_name, "w");
	labelf.write(fp);
   fclose(fp);
#else
   printf("relabel output image\n");
	//	relabel output image and write label description
   ImageT<int> *labelimage;
	labelimage= relabelOutput (out, outimg_name, regdes_name);

   printf("writing output image\n");													
   FILE *fp = fopen(outimg_name, "w");
//   labelf.write(fp);
	labelimage->write(fp);
   fclose(fp);
#endif

	return (1);
}

int main(int argc, char *argv[])
{
  Image labelinput;
  labelinput.read(argv[1]);

  /*
  ImageT<int> *newlabel;
	newlabel= relabelOutput (labelinput, argv[2], argv[4]);

  FILE *fp = fopen(argv[3], "w");
	newlabel->write(fp);
  fclose(fp);
  */
//  Image output=
}
