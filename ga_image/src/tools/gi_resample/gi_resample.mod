<MODULE class=GaImage name=resample
        tip="<p>Resample PFM</p><p>PFM - Portable Float Map</p>"
	cmd="gi_resample @relative@ @mode@ @minmax@ @input@ @output@ @scalex@ @scaley@">
  <INPUT name=input label="Source PFM File: " tip="PFM Input File">
  <OUTPUT name=output depend=input label="Destination PFM File: " tip="PGM Output File">
  <enum name=relative label="Resize mode" options="-r,-a" optionslabel="Rescale,Resize"
        tip="Rescale: new dimension = old dimension times scale<br>Resize: set new dimension in pixel">
  <TEXT name=scalex label="Scale/Size X:" tip="<p>Skalierung des Bildes in X-Richtung</p>">
  <TEXT name=scaley label="Scale/Size Y:" tip="<p>Skalierung des Bildes in Y-Richtung</p>">
  <enum name=mode label="Resample mode:" options=" ,-n,-p" optionslabel="Linear,Nearest Neighbour,Nearest Neighbour Plus">
  <bool name=minmax on="-k" off="" label="Preserve Min/Max-valuse:">
</MODULE>
