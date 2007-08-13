<MODULE class=GaImage name=arithmetic
        tip="<p>Pixelwise arithmetic operations</p>"
	cmd="gi_arithmetic @mode@ @input1@ @input2@ @output@ @verbose@">
  <INPUT name=input1 label="Source File: " tip="Input File">
  <INPUT name=input2 label="Source File: " tip="Input File">
  <OUTPUT name=output depend=input1 label="Destination File: " tip="Output File">
  <ENUM name=mode options="add, sub, mul" optionslabel="Addition, Subtraktion, Multiplikation">	
  <bool name=verbose on="-v" off="">
</MODULE>
