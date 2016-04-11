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
	<Signature URI="#ccme_base" Algorithm="FIPS140_INTEGRITY">MC0CFQCfQoOV8qOkL+D72MNN2RS7OMFpPgIUaj+rGhakXXSx0DiqdnNYOpWYGxQ=</Signature>
	<Signature URI="#ccme_ecc" Algorithm="FIPS140_INTEGRITY">MCwCFF0HoREWNyeecg1xpa8P3wFCfv7BAhRGFg1bPW7QX3ar4iazbyngm60ybQ==</Signature>
	<Signature URI="#ccme_eccaccel" Algorithm="FIPS140_INTEGRITY">MC0CFD5uNlPxGLXRZJJSMGZC6yCgalGCAhUAjGhDjp2ByK/vMSg5LzFxWNQ2vB4=</Signature>
	<Signature URI="#ccme_eccnistaccel" Algorithm="FIPS140_INTEGRITY">MCwCFGh2RHF8CoO97KJNJALzO0otFhW5AhQ8Y6zC5+hLXfL6dbzE/QyOlG+z4A==</Signature>
	<Signature URI="#master" Algorithm="FIPS140_INTEGRITY">MCwCFHcm+ZwC/G0gUkpU3Gm6ru2LUxHJAhRqRt0hPzSi3+5rFiupuLFIV9Lt9g==</Signature>
	<Signature URI="#Crypto-C ME" Algorithm="FIPS140_INTEGRITY">MC0CFQCR0MR6jwOQUMc6tlKttxlPN3bDDwIUXLyyH++wcZmvEJgf5XCrjhujgM0=</Signature>
	<Signature URI="#runtime" Algorithm="FIPS140_INTEGRITY">MCwCFGtuEMrtHD9qQJJUjv4wGIs/VFyYAhRbidTBpa9/HSnjlqiYGa95wyp9NQ==</Signature>
</Configuration>

