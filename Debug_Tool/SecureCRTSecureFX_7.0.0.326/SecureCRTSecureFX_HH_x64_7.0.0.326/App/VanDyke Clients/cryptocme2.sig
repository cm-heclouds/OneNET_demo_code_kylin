<?rsa version="1.0" encoding="utf-8"?>
<Configuration>
	<Product Id="Crypto-C ME">
		<Version>CRYPTO-C ME 3.0.0.0</Version>
		<ReleaseDate>""</ReleaseDate>
		<ExpDate>""</ExpDate>
		<Copyright>
			Copyright (C) RSA
		</Copyright>
		<Library Id="master">cryptocme2</Library>
	</Product>
	<Runtime Id="runtime">
		<LoadOrder>
			<Library Id="ccme_base">ccme_base</Library>
			<Library Id="ccme_ecc">ccme_ecc</Library>
			<Library Id="ccme_eccaccel">ccme_eccaccel</Library>
			<Library Id="ccme_eccnistaccel">ccme_eccnistaccel</Library>
		</LoadOrder>
		<StartupConfig>
			<SelfTest>OnLoad</SelfTest>
		</StartupConfig>
	</Runtime>
	<Signature URI="#ccme_base" Algorithm="FIPS140_INTEGRITY">MC0CFQCjfJf+9+a/aGu8+WegFgDP74m6kgIUNtt8UYCOFLLluuY8PH5T3lwIZMU=</Signature>
	<Signature URI="#ccme_ecc" Algorithm="FIPS140_INTEGRITY">MC0CFDzZ14tgb1QheblrFbVhiVy9HM/jAhUAu5JzYUD4QN57XdONXhKZ3zOBjko=</Signature>
	<Signature URI="#ccme_eccaccel" Algorithm="FIPS140_INTEGRITY">MCwCFG01jKFvePlfTPY3DpjqAW4OWEC5AhRgKJGRo+w1MsCXD3FgpD1vjoJMcA==</Signature>
	<Signature URI="#ccme_eccnistaccel" Algorithm="FIPS140_INTEGRITY">MCwCFGR2xG6hPq1EWe2HlGwPN94uv0gFAhRwzmb5QWfg3QPjMzacNYZiwf/ToQ==</Signature>
	<Signature URI="#master" Algorithm="FIPS140_INTEGRITY">MCwCFCGKtoXczX8F4OTCMeP9nPdoRpyDAhRXifCkgN83ym9wM0bp8ssQjj7Tww==</Signature>
	<Signature URI="#Crypto-C ME" Algorithm="FIPS140_INTEGRITY">MC0CFBkbzz95NsUj+XuXk1awr0z76u0HAhUAvkLny0tnQQEUhbDY6JO1mJfksbo=</Signature>
	<Signature URI="#runtime" Algorithm="FIPS140_INTEGRITY">MC0CFGdVYP0uQx/IyU5iNEVdOsjVUX7lAhUAmOjfRLCZkNEgpiPI+PJFuiw1Vlk=</Signature>
</Configuration>

