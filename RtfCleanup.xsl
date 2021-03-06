<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output method="xml" encoding="UTF-8" standalone="yes" indent="yes"/> 
<xsl:template match="/">
	<rtf>
		<!-- <xsl:apply-templates select="//shpinst"/> -->
		<xsl:apply-templates select="//fonttbl/f|//shpinst">
			<xsl:sort select="shptop/@arg" data-type="number" />
			<xsl:sort select="shpleft/@arg" data-type="number" />
		</xsl:apply-templates>
	</rtf>
</xsl:template>
<xsl:template match="fonttbl/f">
	<font>
		<xsl:attribute name="id"><xsl:value-of select="@arg" /></xsl:attribute>
		<xsl:value-of select="text()" />
	</font>
</xsl:template>
<xsl:template match="//shpinst[string-length(shptxt/f/text()|shptxt/f/line) > 0]">
	<text>
		<xsl:attribute name="top"><xsl:value-of select="shptop/@arg" /></xsl:attribute>
		<xsl:attribute name="left"><xsl:value-of select="shpleft/@arg" /></xsl:attribute>
		<!-- <xsl:attribute name="right"><xsl:value-of select="shpright/@arg" /></xsl:attribute>
		<xsl:attribute name="bottom"><xsl:value-of select="shpbottom/@arg" /></xsl:attribute>-->
		<xsl:attribute name="f"><xsl:value-of select="shptxt/f/@arg" /></xsl:attribute>
		<xsl:attribute name="fs"><xsl:value-of select="shptxt/f/fs/@arg" /></xsl:attribute>
		<xsl:apply-templates select="shptxt/f/text()|shptxt/f/line" />
	</text>
</xsl:template>
<xsl:template match="text()">
	<xsl:value-of select="normalize-space(.)" />
</xsl:template>
<xsl:template match="line">
	<newline />
</xsl:template>
<xsl:template match="*">
</xsl:template>
</xsl:stylesheet>
