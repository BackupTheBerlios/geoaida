<MODULE class=GaImage name=histogram
        tip="<p>Calculate histogram data. Output 1 is ASCII and shows classes, ranges, densities. Output 2 is histogram image.</p>"
	cmd="gi_histogram @verbose@ @slots@ @input@ @output@ @output_image@">
  <INPUT name=input label="Source File: " tip="Input File">
  <OUTPUT name=output depend=input label="Destination File: " tip="ASCII Output File">
  <OUTPUT name=output_image depend=input label="Histogram Image: " tip="Histogram Image File">
  <TEXT name=slots label="Slots:" tip="<p>Number of histogram slots</p>" cmd="--slots @slots@">
  <bool name=verbose on="-v" off="">
</MODULE>
