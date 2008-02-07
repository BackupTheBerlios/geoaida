<MODULE class=GaImage name=Plantation
        tip="<p>Find plantations in sattelite images</p>"
	cmd="gi_plantation @input@ @output@ @meanDistanceScaling@ @maxClusterEdge@ @maxMeanDeviation@">
	<INPUT name=input label="Input Image: ">
	<OUTPUT name=output depend=input label="Output Labelimage: ">
	<TEXT name=meanDistanceScaling label="Feature Scaling Factor: " cmd="@meanDistanceScaling@" value="250.0">
	<TEXT name=maxClusterEdge label="Maximum Cluster Edge: " cmd="@maxClusterEdge@" value="30.0">
	<TEXT name=maxMeanDeviation label="Maximum Mean Deviation: " cmd="@maxMeanDeviation@" value="10000.0">
</MODULE>
