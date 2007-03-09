<MODULE class=GaImage name=regression
        tip="<p>Segmentation</p>"
	cmd="gi_regression @input@ @out@ @mask@ @feature1@ @feature2@">
  <INPUT name=input label="Input: " tip="PFM/PGM/PNM/PBM/PPM-Files are supported">
  <INPUT name=mask label="Mask: " tip="Region Mask">	
  <INPUT name=feature1 label="Learn Feature 1: " tip="Feature Values">
  <INPUT name=feature2 label="Learn Feature 2: " tip="Other Feature Values">
  <OUTPUT name=out depend=input label="Out: " tip="Who knows?">
</MODULE>

