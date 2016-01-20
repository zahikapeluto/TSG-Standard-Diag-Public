# TSG-Standard-Diag-Public
TSG Standard Diagnostic Interface - Public
Revision v1.0


MDMI Implementation Guidelines
A. Sample Code
The attached files serves as sample code for APIs defined in Section 5 of the PRD document TS.31 CR1001.docx
Code in "mdmi_sample_subscribe.c" is most commonly used method of obtaining log records from the component being logged
Code is "mdmi_sample_setgetinvoke.c" provides sample for other APIs such as MdmiGet, MdmiSet and MdmiInvoke

B. MIB Object and Log Record Construction- Background
Issue 1
Some implementers of the MDMI spec have miss-interpreted the applicability of the SNMP standard for encoding OIDs
As a result those implementers were utilizing standard SNMP parsers for generating the structures of the OIDs.  The use of such parsers led to problems in generating code
Implementers are to follow the implementation guidelines in C.1 below.
Issue 2
For proper formatting the MDMI log record, use guidelines in C.2 below

C. Clarifications and Guidelines
The MDMI interface is based on the SNMP standard; however, it is not fully compliant with SNMP for two reasons:

1. The MDMI MIB is not fully compliant with SNMP:
Although the hierarchical structure of the MDMI OIDs follow the structuring rules defined in SNMP/MIB-21, some MDMI OIDs have data types that are not allowed in SNPMP/MIB-2, including:
a.i. Booleans – e.g. ServingCellMeasurementTrapEnabled
a.ii. Special data types – e.g. ServingCellMeasurement data type includes multiple variables; i.e. PCI, RSRP, RSRQ, RSSI, and SINR …
a.ii.1. Note that in this case there is an OID for ServingCellMeasurement, but the individual variables of this structured data type do not have their individual OIDs.
a.ii.2. Also note that these special data types are compliant to ASN.1 syntax.

Due to the above, standard SNMP tools cannot be used for generating the structures of the MDMI OIDs. 

2. The interface between the DM application and the MDMI middleware library should follow the SNMP messaging format, but with the following considerations:
a. A header should contain the length of the whole log record and timestamp.
b. The rest of the log record should include a properly formatted SNMP message including headers and the (OID, value) pairs. However, as per 1 above, some of the values will have data types that do not follow the SNMP standard (these values should be encoded using the rules of ASN.1 BER).

D. Examples

1. The MDMI log record should follow the format: Length + Timestamp + SNMP Message 
2. For Length and Timestamp, refer to the MDMI Definition Document Section 6
3. SNMP Message should be encoded based on SNMP standard and ASN.1 BER. The figure below illustrates a standard SNMP message with actual bytes.


The MDMI log record can encapsulate one of two types of SNMP messages:

a. If the log record corresponds to the return result of an MdmiGet(), then the encapsulated SNMP message would have a PDU Type = GetResponse.

For example,  the GetResponse message for OID “1.3.6.1.2.1.6362.1.1.1” (i.e. for deviceName) and with value “MDMI_TEST_DEVICE” will be encoded as: 303802010004067075626c6963a22b02013f0201000201003020301e060a2b06010201b15a01010104104d444d495f544553545f444556494345. 

Below is a breakdown of this SNMP Message structure (without the MDMI Length and timestamp):
Refer to image at http://imgur.com/xAjjCqW

The OID is encoded as 06 0A 2B 06 01 02 01 B1 5A 01 02 01. 
The first byte 06 is the type of Object Identifier.
The second byte 0A is the length of the following Data field which is 10 (it would be more than one byte when length is larger than 255, about how to encode large number please refer to ASN.1 BER)
Other bytes are for the OID. According to ASN.1 BER, the first two numbers of any OID (x.y) are encoded as one value using the formula (40*x)+y. Therefore, the first two numbers of an SNMP OID are encoded as 43 or 0x2B, because (40*1)+3 = 43. After the first two numbers are encoded, the subsequent numbers in the OID are each encoded as one or more bytes
The Value is encoded as 04 10 4d 44 4d 49 5f 54 45 53 54 5f 44 45 56 49 43 45.
The first byte 04 is the type of Octet String.
The second byte 0A is the length of the following Data field which is 16.
The value of the octet string of “MDMI_TEST_DEVICE”.

b. If the log record corresponds to callback return result of an MdmiSubscribe(), then the encapsulated SNMP message would have a PDU Type = Trap.  Please refer to SNMP standard for the formatting of trap messages.

4. Additional MDMI SNMP message samples
Note: All the following SNMP message samples are only the SNMP messages- to completely construct the return value, ensure that 12 bytes header (length + timestamp) is added as a prefix  to the SNMP message.
a. GetResponse
i. bandIndicator
302902010004067075626c6963a21c02013f0201000201003011300f060a2b06010201b15a01020b020101
ii. neighborCellMeasurements
307402010004067075626c6963a26702013f020100020100305c305a060a2b06010201b15a010202304c302402010a02019c02019c02019c02010a0201050201010201f602019c020192020164020105302402010a02019c02019c02019c02010a0201010201030201f602019b020192020165020103
iii. cqiValue
303102010004067075626c6963a22402013f02010002010030193017060a2b06010201b15a0102073009020100020100020106
iv. rtpCodec
303102010004067075626c6963a22402013f02010002010030193017060a2b06010201b15a0102163009020164020101020101
b. Trap
i. bandIndicator
303b02010004067075626c6963a42e06082b06010201b15a0140047f000001020106020203f343010030123010060b2b06010201b15a01020b00020101
ii. neighborCellMeasurements
30818502010004067075626c6963a47806082b06010201b15a0140047f000001020106020203ea430100305c305a060a2b06010201b15a010202304c302402010a02019c02019c02019c02010a0201050201010201f602019c020192020164020105302402010a02019c02019c02019c02010a0201010201030201f602019b020192020165020103
iii.  cqiValue
304202010004067075626c6963a43506082b06010201b15a0140047f000001020106020203ef43010030193017060a2b06010201b15a0102073009020100020100020106
iv.  rtpCodec
304302010004067075626c6963a43606082b06010201b15a0140047f000001020106020203fe430100301a3018060b2b06010201b15a010216003009020164020101020101

