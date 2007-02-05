<MODULE class=GaImage name=logexp
        tip="<p>Apply logarithm or exponential function to image.</p>"
	cmd="gi_logexp @verbose@ --mode @mode@ @input@ @output@ ">
  <INPUT name=input label="Source file: " tip="Input File">
  <OUTPUT name=output depend=input label="Destination file: " tip="Output File">
  <ENUM name=mode label="Operation: " options="log,exp" optionslabel="LOG,EXP">
  <bool name=verbose on="-v" off="">
</MODULE>
