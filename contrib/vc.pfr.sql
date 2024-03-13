--
-- PostgreSQL database dump
--

SET statement_timeout = 0;
SET lock_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SET check_function_bodies = false;
SET client_min_messages = warning;

--
-- Name: plpgsql; Type: EXTENSION; Schema: -; Owner: 
--

CREATE EXTENSION IF NOT EXISTS plpgsql WITH SCHEMA pg_catalog;


--
-- Name: EXTENSION plpgsql; Type: COMMENT; Schema: -; Owner: 
--

COMMENT ON EXTENSION plpgsql IS 'PL/pgSQL procedural language';


SET search_path = public, pg_catalog;

SET default_tablespace = '';

SET default_with_oids = false;

--
-- Name: adjs; Type: TABLE; Schema: public; Owner: vc; Tablespace: 
--

CREATE TABLE adjs (
    id integer NOT NULL,
    node_id integer NOT NULL,
    adj_node_id integer NOT NULL,
    interface_id_oif integer NOT NULL,
    interface_id_iif integer NOT NULL,
    name character varying(255)
);


ALTER TABLE adjs OWNER TO vc;

--
-- Name: adjs_id_seq; Type: SEQUENCE; Schema: public; Owner: vc
--

CREATE SEQUENCE adjs_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE adjs_id_seq OWNER TO vc;

--
-- Name: adjs_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: vc
--

ALTER SEQUENCE adjs_id_seq OWNED BY adjs.id;


--
-- Name: interface; Type: TABLE; Schema: public; Owner: vc; Tablespace: 
--

CREATE TABLE interface (
    id integer NOT NULL,
    node_id integer NOT NULL,
    type character varying(255),
    name character varying(255),
    description character varying(255)
);


ALTER TABLE interface OWNER TO vc;

--
-- Name: interface_id_seq; Type: SEQUENCE; Schema: public; Owner: vc
--

CREATE SEQUENCE interface_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE interface_id_seq OWNER TO vc;

--
-- Name: interface_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: vc
--

ALTER SEQUENCE interface_id_seq OWNED BY interface.id;


--
-- Name: ipam_addresses; Type: TABLE; Schema: public; Owner: vc; Tablespace: 
--

CREATE TABLE ipam_addresses (
    id integer NOT NULL,
    created date,
    last_updated timestamp without time zone,
    type character varying(255),
    family integer NOT NULL,
    address character varying(255),
    description character varying(255),
    interface_id integer NOT NULL,
    vrf_id integer NOT NULL,
    status integer NOT NULL
);


ALTER TABLE ipam_addresses OWNER TO vc;

--
-- Name: ipam_addresses_id_seq; Type: SEQUENCE; Schema: public; Owner: vc
--

CREATE SEQUENCE ipam_addresses_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE ipam_addresses_id_seq OWNER TO vc;

--
-- Name: ipam_addresses_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: vc
--

ALTER SEQUENCE ipam_addresses_id_seq OWNED BY ipam_addresses.id;


--
-- Name: ipam_vrf; Type: TABLE; Schema: public; Owner: vc; Tablespace: 
--

CREATE TABLE ipam_vrf (
    id integer NOT NULL,
    created date,
    last_updated timestamp without time zone,
    name character varying(255),
    rd character varying(255),
    description character varying(255)
);


ALTER TABLE ipam_vrf OWNER TO vc;

--
-- Name: ipam_vrf_id_seq; Type: SEQUENCE; Schema: public; Owner: vc
--

CREATE SEQUENCE ipam_vrf_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE ipam_vrf_id_seq OWNER TO vc;

--
-- Name: ipam_vrf_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: vc
--

ALTER SEQUENCE ipam_vrf_id_seq OWNED BY ipam_vrf.id;


--
-- Name: node; Type: TABLE; Schema: public; Owner: vc; Tablespace: 
--

CREATE TABLE node (
    id integer NOT NULL,
    type_id integer,
    hostname character varying(255),
    ip character varying(15),
    id_sg_node integer,
    pfr_vplsip integer,
    is_asbr integer
);


ALTER TABLE node OWNER TO vc;

--
-- Name: node_id_seq; Type: SEQUENCE; Schema: public; Owner: vc
--

CREATE SEQUENCE node_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE node_id_seq OWNER TO vc;

--
-- Name: node_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: vc
--

ALTER SEQUENCE node_id_seq OWNED BY node.id;


--
-- Name: pfr_case; Type: TABLE; Schema: public; Owner: vc; Tablespace: 
--

CREATE TABLE pfr_case (
    id integer NOT NULL,
    name character varying(255)
);


ALTER TABLE pfr_case OWNER TO vc;

--
-- Name: pfr_customer; Type: TABLE; Schema: public; Owner: vc; Tablespace: 
--

CREATE TABLE pfr_customer (
    id integer NOT NULL,
    description character varying(255),
    type_id integer NOT NULL,
    autnum_id integer,
    asset_id character varying(255)
);


ALTER TABLE pfr_customer OWNER TO vc;

--
-- Name: pfr_customer_type; Type: TABLE; Schema: public; Owner: vc; Tablespace: 
--

CREATE TABLE pfr_customer_type (
    id integer NOT NULL,
    description character varying(255)
);


ALTER TABLE pfr_customer_type OWNER TO vc;

--
-- Name: pfr_customer_type_id_seq; Type: SEQUENCE; Schema: public; Owner: vc
--

CREATE SEQUENCE pfr_customer_type_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE pfr_customer_type_id_seq OWNER TO vc;

--
-- Name: pfr_customer_type_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: vc
--

ALTER SEQUENCE pfr_customer_type_id_seq OWNED BY pfr_customer_type.id;


--
-- Name: pfr_dscp; Type: TABLE; Schema: public; Owner: vc; Tablespace: 
--

CREATE TABLE pfr_dscp (
    id integer NOT NULL,
    dscp integer,
    tos integer,
    ipp character varying(255),
    phb character varying(255)
);


ALTER TABLE pfr_dscp OWNER TO vc;

--
-- Name: pfr_dscp_id_seq; Type: SEQUENCE; Schema: public; Owner: vc
--

CREATE SEQUENCE pfr_dscp_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE pfr_dscp_id_seq OWNER TO vc;

--
-- Name: pfr_dscp_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: vc
--

ALTER SEQUENCE pfr_dscp_id_seq OWNED BY pfr_dscp.id;


--
-- Name: pfr_dst; Type: TABLE; Schema: public; Owner: vc; Tablespace: 
--

CREATE TABLE pfr_dst (
    id integer NOT NULL,
    ipv4_dst_address character varying(255),
    ipv6_dst_address character varying(255),
    description character varying(255)
);


ALTER TABLE pfr_dst OWNER TO vc;

--
-- Name: pfr_dst_id_seq; Type: SEQUENCE; Schema: public; Owner: vc
--

CREATE SEQUENCE pfr_dst_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE pfr_dst_id_seq OWNER TO vc;

--
-- Name: pfr_dst_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: vc
--

ALTER SEQUENCE pfr_dst_id_seq OWNED BY pfr_dst.id;


--
-- Name: pfr_ipx; Type: TABLE; Schema: public; Owner: vc; Tablespace: 
--

CREATE TABLE pfr_ipx (
    id integer NOT NULL,
    prefix character varying(255),
    aut_num integer,
    isdst integer
);


ALTER TABLE pfr_ipx OWNER TO vc;

--
-- Name: pfr_log; Type: TABLE; Schema: public; Owner: vc; Tablespace: 
--

CREATE TABLE pfr_log (
    id integer NOT NULL,
    datetime timestamp with time zone,
    ccase_id integer,
    probe_id integer,
    peer_id integer NOT NULL,
    ipv4_dest_address character varying(255),
    ipv6_dest_address character varying(255),
    pkts integer NOT NULL,
    rtt_min real NOT NULL,
    rtt_avg real NOT NULL,
    pkt_loss integer,
    ts integer NOT NULL
);


ALTER TABLE pfr_log OWNER TO vc;

--
-- Name: pfr_log_id_seq; Type: SEQUENCE; Schema: public; Owner: vc
--

CREATE SEQUENCE pfr_log_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE pfr_log_id_seq OWNER TO vc;

--
-- Name: pfr_log_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: vc
--

ALTER SEQUENCE pfr_log_id_seq OWNED BY pfr_log.id;


--
-- Name: pfr_peer_group; Type: TABLE; Schema: public; Owner: vc; Tablespace: 
--

CREATE TABLE pfr_peer_group (
    id integer NOT NULL,
    ipv4_address character varying(255),
    ipv6_address character varying(255),
    ipv4_nh_address character varying(255),
    ipv6_nh_address character varying(255),
    description character varying(255)
);


ALTER TABLE pfr_peer_group OWNER TO vc;

--
-- Name: pfr_peers; Type: TABLE; Schema: public; Owner: vc; Tablespace: 
--

CREATE TABLE pfr_peers (
    id integer NOT NULL,
    peer_group_id integer NOT NULL,
    node_id integer,
    interface_id integer,
    address_id integer,
    ipv4_peer_address character varying(255),
    ipv6_peer_address character varying(255),
    type character varying(255),
    name character varying(255),
    dscp_id integer,
    description character varying(255),
    pfr_dst_id integer,
    laddress_id integer,
    lipv4_peer_address character varying(255),
    lipv6_peer_address character varying(255),
    peer_community character varying(255)
);


ALTER TABLE pfr_peers OWNER TO vc;

--
-- Name: pfr_peers_id_seq; Type: SEQUENCE; Schema: public; Owner: vc
--

CREATE SEQUENCE pfr_peers_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE pfr_peers_id_seq OWNER TO vc;

--
-- Name: pfr_peers_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: vc
--

ALTER SEQUENCE pfr_peers_id_seq OWNED BY pfr_peers.id;


--
-- Name: pfr_prefix4; Type: TABLE; Schema: public; Owner: vc; Tablespace: 
--

CREATE TABLE pfr_prefix4 (
    id integer NOT NULL,
    customer_id integer NOT NULL,
    prefix character varying(255)
);


ALTER TABLE pfr_prefix4 OWNER TO vc;

--
-- Name: pfr_prefix4_id_seq; Type: SEQUENCE; Schema: public; Owner: vc
--

CREATE SEQUENCE pfr_prefix4_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE pfr_prefix4_id_seq OWNER TO vc;

--
-- Name: pfr_prefix4_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: vc
--

ALTER SEQUENCE pfr_prefix4_id_seq OWNED BY pfr_prefix4.id;


--
-- Name: pfr_prefix6; Type: TABLE; Schema: public; Owner: vc; Tablespace: 
--

CREATE TABLE pfr_prefix6 (
    id integer NOT NULL,
    customer_id integer NOT NULL,
    prefix character varying(255)
);


ALTER TABLE pfr_prefix6 OWNER TO vc;

--
-- Name: pfr_prefix6_id_seq; Type: SEQUENCE; Schema: public; Owner: vc
--

CREATE SEQUENCE pfr_prefix6_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE pfr_prefix6_id_seq OWNER TO vc;

--
-- Name: pfr_prefix6_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: vc
--

ALTER SEQUENCE pfr_prefix6_id_seq OWNED BY pfr_prefix6.id;


--
-- Name: pfr_results_mesurements; Type: TABLE; Schema: public; Owner: vc; Tablespace: 
--

CREATE TABLE pfr_results_mesurements (
    id integer NOT NULL,
    ipv4_dest_address character varying(255),
    ipv6_dest_address character varying(255),
    pkts integer NOT NULL,
    rtt_min integer NOT NULL,
    rtt_avg integer NOT NULL,
    rtt_max integer NOT NULL,
    rtt_mdev integer NOT NULL,
    pkt_loss integer NOT NULL,
    delay integer NOT NULL,
    jitter integer NOT NULL,
    latency integer NOT NULL,
    pfr_peers_id integer NOT NULL
);


ALTER TABLE pfr_results_mesurements OWNER TO vc;

--
-- Name: pfr_results_mesurements_id_seq; Type: SEQUENCE; Schema: public; Owner: vc
--

CREATE SEQUENCE pfr_results_mesurements_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE pfr_results_mesurements_id_seq OWNER TO vc;

--
-- Name: pfr_results_mesurements_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: vc
--

ALTER SEQUENCE pfr_results_mesurements_id_seq OWNED BY pfr_results_mesurements.id;


--
-- Name: sub_int_roles; Type: TABLE; Schema: public; Owner: vc; Tablespace: 
--

CREATE TABLE sub_int_roles (
    id integer NOT NULL,
    role character varying(255)
);


ALTER TABLE sub_int_roles OWNER TO vc;

--
-- Name: sub_int_roles_id_seq; Type: SEQUENCE; Schema: public; Owner: vc
--

CREATE SEQUENCE sub_int_roles_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE sub_int_roles_id_seq OWNER TO vc;

--
-- Name: sub_int_roles_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: vc
--

ALTER SEQUENCE sub_int_roles_id_seq OWNED BY sub_int_roles.id;


--
-- Name: sub_interfaces; Type: TABLE; Schema: public; Owner: vc; Tablespace: 
--

CREATE TABLE sub_interfaces (
    id integer NOT NULL,
    parent_id integer,
    type character varying(255),
    intf character varying(255),
    unit integer NOT NULL,
    role_id integer NOT NULL,
    description character varying(255),
    snmp_idx integer,
    intf_idx integer
);


ALTER TABLE sub_interfaces OWNER TO vc;

--
-- Name: sub_interfaces_id_seq; Type: SEQUENCE; Schema: public; Owner: vc
--

CREATE SEQUENCE sub_interfaces_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE sub_interfaces_id_seq OWNER TO vc;

--
-- Name: sub_interfaces_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: vc
--

ALTER SEQUENCE sub_interfaces_id_seq OWNED BY sub_interfaces.id;


--
-- Name: type; Type: TABLE; Schema: public; Owner: vc; Tablespace: 
--

CREATE TABLE type (
    id integer NOT NULL,
    type character varying(255),
    role character varying(255),
    name character varying(255)
);


ALTER TABLE type OWNER TO vc;

--
-- Name: type_id_seq; Type: SEQUENCE; Schema: public; Owner: vc
--

CREATE SEQUENCE type_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE type_id_seq OWNER TO vc;

--
-- Name: type_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: vc
--

ALTER SEQUENCE type_id_seq OWNED BY type.id;


--
-- Name: vc; Type: TABLE; Schema: public; Owner: vc; Tablespace: 
--

CREATE TABLE vc (
    id integer NOT NULL,
    circuit_id integer NOT NULL,
    vlan_id_outer integer NOT NULL,
    vlan_id_inner integer NOT NULL,
    description character varying(255),
    vc_type_id integer NOT NULL
);


ALTER TABLE vc OWNER TO vc;

--
-- Name: vc_type; Type: TABLE; Schema: public; Owner: vc; Tablespace: 
--

CREATE TABLE vc_type (
    id integer NOT NULL,
    type character varying(255)
);


ALTER TABLE vc_type OWNER TO vc;

--
-- Name: vlans; Type: TABLE; Schema: public; Owner: vc; Tablespace: 
--

CREATE TABLE vlans (
    id integer NOT NULL,
    interface_id integer NOT NULL,
    node_id integer NOT NULL,
    unit integer NOT NULL,
    vid integer NOT NULL,
    vid_range0 integer NOT NULL,
    vid_range1 integer NOT NULL,
    name character varying(255),
    sub_interface_id integer NOT NULL
);


ALTER TABLE vlans OWNER TO vc;

--
-- Name: vrf; Type: TABLE; Schema: public; Owner: vc; Tablespace: 
--

CREATE TABLE vrf (
    id integer NOT NULL,
    description character varying(255),
    name character varying(255),
    vrf_type_id integer NOT NULL,
    rd integer NOT NULL,
    rt_import integer NOT NULL,
    rt_export integer NOT NULL,
    ltint character varying(255),
    net character varying(255),
    vlan_id integer NOT NULL
);


ALTER TABLE vrf OWNER TO vc;

--
-- Name: vrf_id_seq; Type: SEQUENCE; Schema: public; Owner: vc
--

CREATE SEQUENCE vrf_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE vrf_id_seq OWNER TO vc;

--
-- Name: vrf_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: vc
--

ALTER SEQUENCE vrf_id_seq OWNED BY vrf.id;


--
-- Name: vrf_nodes; Type: TABLE; Schema: public; Owner: vc; Tablespace: 
--

CREATE TABLE vrf_nodes (
    id integer NOT NULL,
    vrf_id integer NOT NULL,
    node_id integer NOT NULL
);


ALTER TABLE vrf_nodes OWNER TO vc;

--
-- Name: vrf_nodes_id_seq; Type: SEQUENCE; Schema: public; Owner: vc
--

CREATE SEQUENCE vrf_nodes_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE vrf_nodes_id_seq OWNER TO vc;

--
-- Name: vrf_nodes_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: vc
--

ALTER SEQUENCE vrf_nodes_id_seq OWNED BY vrf_nodes.id;


--
-- Name: vrf_type; Type: TABLE; Schema: public; Owner: vc; Tablespace: 
--

CREATE TABLE vrf_type (
    id integer NOT NULL,
    type character varying(255),
    description character varying(255)
);


ALTER TABLE vrf_type OWNER TO vc;

--
-- Name: vrf_type_id_seq; Type: SEQUENCE; Schema: public; Owner: vc
--

CREATE SEQUENCE vrf_type_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE vrf_type_id_seq OWNER TO vc;

--
-- Name: vrf_type_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: vc
--

ALTER SEQUENCE vrf_type_id_seq OWNED BY vrf_type.id;


--
-- Name: vrf_vrf; Type: TABLE; Schema: public; Owner: vc; Tablespace: 
--

CREATE TABLE vrf_vrf (
    id integer NOT NULL,
    vrf_id0 integer NOT NULL,
    vrf_id1 integer NOT NULL,
    vrf_id2 integer NOT NULL
);


ALTER TABLE vrf_vrf OWNER TO vc;

--
-- Name: vrf_vrf_id_seq; Type: SEQUENCE; Schema: public; Owner: vc
--

CREATE SEQUENCE vrf_vrf_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE vrf_vrf_id_seq OWNER TO vc;

--
-- Name: vrf_vrf_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: vc
--

ALTER SEQUENCE vrf_vrf_id_seq OWNED BY vrf_vrf.id;


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: vc
--

ALTER TABLE ONLY adjs ALTER COLUMN id SET DEFAULT nextval('adjs_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: vc
--

ALTER TABLE ONLY interface ALTER COLUMN id SET DEFAULT nextval('interface_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: vc
--

ALTER TABLE ONLY ipam_addresses ALTER COLUMN id SET DEFAULT nextval('ipam_addresses_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: vc
--

ALTER TABLE ONLY ipam_vrf ALTER COLUMN id SET DEFAULT nextval('ipam_vrf_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: vc
--

ALTER TABLE ONLY node ALTER COLUMN id SET DEFAULT nextval('node_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: vc
--

ALTER TABLE ONLY pfr_customer_type ALTER COLUMN id SET DEFAULT nextval('pfr_customer_type_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: vc
--

ALTER TABLE ONLY pfr_dscp ALTER COLUMN id SET DEFAULT nextval('pfr_dscp_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: vc
--

ALTER TABLE ONLY pfr_dst ALTER COLUMN id SET DEFAULT nextval('pfr_dst_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: vc
--

ALTER TABLE ONLY pfr_log ALTER COLUMN id SET DEFAULT nextval('pfr_log_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: vc
--

ALTER TABLE ONLY pfr_peers ALTER COLUMN id SET DEFAULT nextval('pfr_peers_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: vc
--

ALTER TABLE ONLY pfr_prefix4 ALTER COLUMN id SET DEFAULT nextval('pfr_prefix4_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: vc
--

ALTER TABLE ONLY pfr_prefix6 ALTER COLUMN id SET DEFAULT nextval('pfr_prefix6_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: vc
--

ALTER TABLE ONLY pfr_results_mesurements ALTER COLUMN id SET DEFAULT nextval('pfr_results_mesurements_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: vc
--

ALTER TABLE ONLY sub_int_roles ALTER COLUMN id SET DEFAULT nextval('sub_int_roles_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: vc
--

ALTER TABLE ONLY sub_interfaces ALTER COLUMN id SET DEFAULT nextval('sub_interfaces_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: vc
--

ALTER TABLE ONLY type ALTER COLUMN id SET DEFAULT nextval('type_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: vc
--

ALTER TABLE ONLY vrf ALTER COLUMN id SET DEFAULT nextval('vrf_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: vc
--

ALTER TABLE ONLY vrf_nodes ALTER COLUMN id SET DEFAULT nextval('vrf_nodes_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: vc
--

ALTER TABLE ONLY vrf_type ALTER COLUMN id SET DEFAULT nextval('vrf_type_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: vc
--

ALTER TABLE ONLY vrf_vrf ALTER COLUMN id SET DEFAULT nextval('vrf_vrf_id_seq'::regclass);


--
-- Data for Name: adjs; Type: TABLE DATA; Schema: public; Owner: vc
--

COPY adjs (id, node_id, adj_node_id, interface_id_oif, interface_id_iif, name) FROM stdin;
1018	206	207	329	330	CE3:null<-->null:null
1019	206	208	331	332	CE3:ge-0/0/3<-->srx-noc:ge-0/0/2
1020	206	209	333	334	CE3:ge-0/0/1<-->PE1:ge-0/0/5
1021	206	210	335	336	CE3:ge-0/0/2<-->PE3:ge-0/0/5
1022	211	212	337	338	SP0-RR1:ge-0/0/0<-->CE2:ge-0/0/6
1023	213	212	339	340	PE2:ge-0/0/2<-->CE2:ge-0/0/0
1024	213	214	341	342	PE2:ge-0/0/1<-->core0:ge-0/0/4
1025	213	215	343	344	PE2:ge-0/0/5<-->PE0:ge-0/0/5
1026	213	216	345	346	PE2:ge-0/0/3<-->CE0:ge-0/0/1
1027	217	216	347	348	SP0-PE0:ge-0/0/0<-->CE0:ge-0/0/7
1028	210	206	349	350	PE3:ge-0/0/7<-->CE3:ge-0/0/4
1029	210	218	351	352	PE3:ge-0/0/1<-->core1:ge-0/0/4
1030	210	209	353	354	PE3:ge-0/0/8<-->PE1:ge-0/0/9
1031	219	216	355	356	SP0-RR0:ge-0/0/0<-->CE0:ge-0/0/6
1032	220	207	357	330	noc-sw:null<-->null:null
1033	220	218	358	359	noc-sw:xe-0/0/0<-->core1:ge-0/0/5
1034	220	208	360	361	noc-sw:xe-0/0/2<-->srx-noc:ge-0/0/0
1035	218	214	362	363	core1:ge-0/0/0<-->core0:ge-0/0/0
1036	218	215	364	365	core1:ge-0/0/2<-->PE0:ge-0/0/0
1037	218	209	366	367	core1:ge-0/0/3<-->PE1:ge-0/0/1
1038	218	210	352	351	core1:ge-0/0/4<-->PE3:ge-0/0/1
1039	208	220	361	360	srx-noc:ge-0/0/0<-->noc-sw:xe-0/0/2
1040	215	212	368	369	PE0:ge-0/0/4<-->CE2:ge-0/0/1
1041	215	221	370	371	PE0:ge-0/0/6<-->bras0:ge-0/0/0
1042	215	218	365	364	PE0:ge-0/0/0<-->core1:ge-0/0/2
1043	215	214	372	373	PE0:ge-0/0/1<-->core0:ge-0/0/2
1044	215	213	344	343	PE0:ge-0/0/5<-->PE2:ge-0/0/5
1045	215	216	374	375	PE0:ge-0/0/3<-->CE0:ge-0/0/0
1046	222	212	376	377	SP0-PE1:ge-0/0/0<-->CE2:ge-0/0/7
1047	212	222	377	376	CE2:ge-0/0/7<-->SP0-PE1:ge-0/0/0
1048	212	211	338	337	CE2:ge-0/0/6<-->SP0-RR1:ge-0/0/0
1049	212	215	369	368	CE2:ge-0/0/1<-->PE0:ge-0/0/4
1050	212	213	340	339	CE2:ge-0/0/0<-->PE2:ge-0/0/2
1051	212	216	378	379	CE2:ge-0/0/3<-->CE0:ge-0/0/3
1052	223	224	380	381	bras1:ge-0/0/0<-->access00:ge-0/0/3
1053	223	209	382	383	bras1:ge-0/0/4<-->PE1:ge-0/0/7
1054	216	212	379	378	CE0:ge-0/0/3<-->CE2:ge-0/0/3
1055	216	217	348	347	CE0:ge-0/0/7<-->SP0-PE0:ge-0/0/0
1056	216	219	356	355	CE0:ge-0/0/6<-->SP0-RR0:ge-0/0/0
1057	216	215	375	374	CE0:ge-0/0/0<-->PE0:ge-0/0/3
1058	216	213	346	345	CE0:ge-0/0/1<-->PE2:ge-0/0/3
1059	214	218	363	362	core0:ge-0/0/0<-->core1:ge-0/0/0
1060	214	215	373	372	core0:ge-0/0/2<-->PE0:ge-0/0/1
1061	214	213	342	341	core0:ge-0/0/4<-->PE2:ge-0/0/1
1062	214	209	384	385	core0:ge-0/0/3<-->PE1:ge-0/0/0
1063	221	215	371	370	bras0:ge-0/0/0<-->PE0:ge-0/0/6
1064	221	224	386	387	bras0:ge-0/0/4<-->access00:ge-0/0/0
1065	221	209	388	389	bras0:ge-0/0/1<-->PE1:ge-0/0/6
1066	224	221	387	386	access00:ge-0/0/0<-->bras0:ge-0/0/4
1067	224	223	390	391	access00:ge-0/0/2<-->bras1:ge-0/0/3
1068	209	206	334	333	PE1:ge-0/0/5<-->CE3:ge-0/0/1
1069	209	221	389	388	PE1:ge-0/0/6<-->bras0:ge-0/0/1
1070	209	223	383	382	PE1:ge-0/0/7<-->bras1:ge-0/0/4
1071	209	214	385	384	PE1:ge-0/0/0<-->core0:ge-0/0/3
1072	209	225	392	393	PE1:ge-0/0/3<-->CE1:ge-0/0/0
1073	209	210	354	353	PE1:ge-0/0/9<-->PE3:ge-0/0/8
1074	225	220	394	395	CE1:ge-0/0/6<-->noc-sw:xe-0/0/5
1075	225	209	393	392	CE1:ge-0/0/0<-->PE1:ge-0/0/3
\.


--
-- Name: adjs_id_seq; Type: SEQUENCE SET; Schema: public; Owner: vc
--

SELECT pg_catalog.setval('adjs_id_seq', 1075, true);


--
-- Data for Name: interface; Type: TABLE DATA; Schema: public; Owner: vc
--

COPY interface (id, node_id, type, name, description) FROM stdin;
329	206	\N	null	\N
330	207	\N	null	\N
331	206	\N	ge-0/0/3	\N
332	208	\N	ge-0/0/2	\N
333	206	\N	ge-0/0/1	\N
334	209	\N	ge-0/0/5	\N
335	206	\N	ge-0/0/2	\N
336	210	\N	ge-0/0/5	\N
337	211	\N	ge-0/0/0	\N
338	212	\N	ge-0/0/6	\N
339	213	\N	ge-0/0/2	\N
340	212	\N	ge-0/0/0	\N
341	213	\N	ge-0/0/1	\N
342	214	\N	ge-0/0/4	\N
343	213	\N	ge-0/0/5	\N
344	215	\N	ge-0/0/5	\N
345	213	\N	ge-0/0/3	\N
346	216	\N	ge-0/0/1	\N
347	217	\N	ge-0/0/0	\N
348	216	\N	ge-0/0/7	\N
349	210	\N	ge-0/0/7	\N
350	206	\N	ge-0/0/4	\N
351	210	\N	ge-0/0/1	\N
352	218	\N	ge-0/0/4	\N
353	210	\N	ge-0/0/8	\N
354	209	\N	ge-0/0/9	\N
355	219	\N	ge-0/0/0	\N
356	216	\N	ge-0/0/6	\N
357	220	\N	null	\N
358	220	\N	xe-0/0/0	\N
359	218	\N	ge-0/0/5	\N
360	220	\N	xe-0/0/2	\N
361	208	\N	ge-0/0/0	\N
362	218	\N	ge-0/0/0	\N
363	214	\N	ge-0/0/0	\N
364	218	\N	ge-0/0/2	\N
365	215	\N	ge-0/0/0	\N
366	218	\N	ge-0/0/3	\N
367	209	\N	ge-0/0/1	\N
368	215	\N	ge-0/0/4	\N
369	212	\N	ge-0/0/1	\N
370	215	\N	ge-0/0/6	\N
371	221	\N	ge-0/0/0	\N
372	215	\N	ge-0/0/1	\N
373	214	\N	ge-0/0/2	\N
374	215	\N	ge-0/0/3	\N
375	216	\N	ge-0/0/0	\N
376	222	\N	ge-0/0/0	\N
377	212	\N	ge-0/0/7	\N
378	212	\N	ge-0/0/3	\N
379	216	\N	ge-0/0/3	\N
380	223	\N	ge-0/0/0	\N
381	224	\N	ge-0/0/3	\N
382	223	\N	ge-0/0/4	\N
383	209	\N	ge-0/0/7	\N
384	214	\N	ge-0/0/3	\N
385	209	\N	ge-0/0/0	\N
386	221	\N	ge-0/0/4	\N
387	224	\N	ge-0/0/0	\N
388	221	\N	ge-0/0/1	\N
389	209	\N	ge-0/0/6	\N
390	224	\N	ge-0/0/2	\N
391	223	\N	ge-0/0/3	\N
392	209	\N	ge-0/0/3	\N
393	225	\N	ge-0/0/0	\N
394	225	\N	ge-0/0/6	\N
395	220	\N	xe-0/0/5	\N
396	215	\N	gr-0/0/10	\N
397	209	\N	gr-0/0/10	\N
398	226	\N	eth0	eth0
399	227	\N	eth0	eth0
0	0	fake	fake	fake
\.


--
-- Name: interface_id_seq; Type: SEQUENCE SET; Schema: public; Owner: vc
--

SELECT pg_catalog.setval('interface_id_seq', 399, true);


--
-- Data for Name: ipam_addresses; Type: TABLE DATA; Schema: public; Owner: vc
--

COPY ipam_addresses (id, created, last_updated, type, family, address, description, interface_id, vrf_id, status) FROM stdin;
1	\N	\N	uplink	4	198.51.0.0/31	ISP0	1	2	1
2	\N	\N	uplink	4	198.51.0.2/31	ISP1	2	2	1
3	\N	\N	uplink	4	198.51.0.24/31	ISP01	3	2	1
4	\N	\N	uplink	4	198.51.0.26/31	ISP02	4	2	1
6	\N	\N	uplink	4	198.51.0.30/31	ISP13	6	2	1
5	\N	\N	uplink	4	198.51.0.28/31	ISP12	5	2	1
7	\N	\N	uplink	4	198.51.0.32/31	ISP4	10	2	1
8	\N	\N	uplink	4	198.51.0.34/31	ISP5	11	2	1
0	\N	\N	\N	4	\N	\N	0	2	1
\.


--
-- Name: ipam_addresses_id_seq; Type: SEQUENCE SET; Schema: public; Owner: vc
--

SELECT pg_catalog.setval('ipam_addresses_id_seq', 8, true);


--
-- Data for Name: ipam_vrf; Type: TABLE DATA; Schema: public; Owner: vc
--

COPY ipam_vrf (id, created, last_updated, name, rd, description) FROM stdin;
1	\N	\N	default	\N	\N
2	\N	\N	i	3333:1	\N
\.


--
-- Name: ipam_vrf_id_seq; Type: SEQUENCE SET; Schema: public; Owner: vc
--

SELECT pg_catalog.setval('ipam_vrf_id_seq', 2, true);


--
-- Data for Name: node; Type: TABLE DATA; Schema: public; Owner: vc
--

COPY node (id, type_id, hostname, ip, id_sg_node, pfr_vplsip, is_asbr) FROM stdin;
206	1	CE3	10.248.0.66	0	\N	\N
207	1	null	0.0.0.0	\N	\N	\N
208	1	srx-noc	10.229.138.0	8	\N	\N
211	1	SP0-RR1	172.31.0.254	1	\N	\N
212	1	CE2	10.240.0.66	11	\N	\N
214	1	core0	10.229.0.0	14	\N	\N
216	1	CE0	10.240.0.65	13	\N	\N
217	1	SP0-PE0	172.31.0.253	3	\N	\N
218	1	core1	10.229.128.0	7	\N	\N
219	1	SP0-RR0	172.31.0.255	5	\N	\N
220	1	noc-sw	10.229.136.0	6	\N	\N
221	1	bras0	10.229.140.0	15	\N	\N
222	1	SP0-PE1	172.31.0.252	10	\N	\N
223	1	bras1	10.229.150.0	12	\N	\N
224	1	access00	10.229.142.0	16	\N	\N
225	1	CE1	10.248.0.4	18	\N	\N
226	1	IPFIX0	127.0.0.1	\N	\N	\N
227	1	IPFIX1	192.168.122.19	\N	\N	\N
0	0	fake	\N	\N	\N	\N
213	1	PE2	10.229.6.0	2	2	\N
210	1	PE3	10.229.134.0	4	3	\N
215	1	PE0	10.229.4.0	9	10	1
209	1	PE1	10.229.132.0	17	1	1
\.


--
-- Name: node_id_seq; Type: SEQUENCE SET; Schema: public; Owner: vc
--

SELECT pg_catalog.setval('node_id_seq', 227, true);


--
-- Data for Name: pfr_case; Type: TABLE DATA; Schema: public; Owner: vc
--

COPY pfr_case (id, name) FROM stdin;
20	case 2.0 no answer from dst
21	case 2.1 there is answer from dst
11	case 1.1 there is not prev answer from dst
10	case 1.0 first probe id 0
\.


--
-- Data for Name: pfr_customer; Type: TABLE DATA; Schema: public; Owner: vc
--

COPY pfr_customer (id, description, type_id, autnum_id, asset_id) FROM stdin;
7	\N	1	\N	\N
\.


--
-- Data for Name: pfr_customer_type; Type: TABLE DATA; Schema: public; Owner: vc
--

COPY pfr_customer_type (id, description) FROM stdin;
1	connected
2	ebgp
\.


--
-- Name: pfr_customer_type_id_seq; Type: SEQUENCE SET; Schema: public; Owner: vc
--

SELECT pg_catalog.setval('pfr_customer_type_id_seq', 2, true);


--
-- Data for Name: pfr_dscp; Type: TABLE DATA; Schema: public; Owner: vc
--

COPY pfr_dscp (id, dscp, tos, ipp, phb) FROM stdin;
1	56	224	\N	\N
2	48	192	\N	\N
3	46	184	\N	\N
4	40	160	\N	\N
5	38	152	\N	\N
6	36	144	\N	\N
7	34	136	\N	\N
8	32	128	\N	\N
9	30	120	\N	\N
10	28	112	\N	\N
11	26	104	\N	\N
12	24	96	\N	\N
13	22	88	\N	\N
14	20	80	\N	\N
15	18	72	\N	\N
16	16	64	\N	\N
17	14	56	\N	\N
18	12	48	\N	\N
19	10	40	\N	\N
20	8	32	\N	\N
21	0	0	\N	\N
\.


--
-- Name: pfr_dscp_id_seq; Type: SEQUENCE SET; Schema: public; Owner: vc
--

SELECT pg_catalog.setval('pfr_dscp_id_seq', 21, true);


--
-- Data for Name: pfr_dst; Type: TABLE DATA; Schema: public; Owner: vc
--

COPY pfr_dst (id, ipv4_dst_address, ipv6_dst_address, description) FROM stdin;
10002	62.192.1.28	\N	\N
10003	62.192.1.29	\N	\N
10004	62.192.1.30	\N	\N
10001	62.192.1.27	\N	\N
\.


--
-- Name: pfr_dst_id_seq; Type: SEQUENCE SET; Schema: public; Owner: vc
--

SELECT pg_catalog.setval('pfr_dst_id_seq', 10004, true);


--
-- Data for Name: pfr_ipx; Type: TABLE DATA; Schema: public; Owner: vc
--

COPY pfr_ipx (id, prefix, aut_num, isdst) FROM stdin;
1	60.128.128.0/17	3333	0
2	60.128.0.0/15	3333	0
2	6.128.0.0/15	333	1
\.


--
-- Data for Name: pfr_log; Type: TABLE DATA; Schema: public; Owner: vc
--

COPY pfr_log (id, datetime, ccase_id, probe_id, peer_id, ipv4_dest_address, ipv6_dest_address, pkts, rtt_min, rtt_avg, pkt_loss, ts) FROM stdin;
\.


--
-- Name: pfr_log_id_seq; Type: SEQUENCE SET; Schema: public; Owner: vc
--

SELECT pg_catalog.setval('pfr_log_id_seq', 5391582, true);


--
-- Data for Name: pfr_peer_group; Type: TABLE DATA; Schema: public; Owner: vc
--

COPY pfr_peer_group (id, ipv4_address, ipv6_address, ipv4_nh_address, ipv6_nh_address, description) FROM stdin;
8	198.51.0.244/31	\N	198.51.0.245	\N	pinger8
7	198.51.0.246/31	\N	198.51.0.247	\N	pinger7
3	198.51.0.248/31	\N	198.51.0.249	\N	pinger3
2	198.51.0.250/31	\N	198.51.0.251	\N	pinger2
1	198.51.0.252/31	\N	198.51.0.253	\N	pinger1
0	198.51.0.254/31	\N	198.51.0.255	\N	pinger0
\.


--
-- Data for Name: pfr_peers; Type: TABLE DATA; Schema: public; Owner: vc
--

COPY pfr_peers (id, peer_group_id, node_id, interface_id, address_id, ipv4_peer_address, ipv6_peer_address, type, name, dscp_id, description, pfr_dst_id, laddress_id, lipv4_peer_address, lipv6_peer_address, peer_community) FROM stdin;
4	1	209	2	2	198.51.0.3	\N	uplink	ISP1	20	ISP1	0	\N	\N	\N	3333:38040
6	0	215	4	4	198.51.0.27	\N	uplink	ISP02	18	ISP02	0	\N	\N	\N	3333:38060
5	0	215	3	3	198.51.0.25	\N	uplink	ISP01	19	ISP01	0	\N	\N	\N	3333:38050
7	1	209	5	5	198.51.0.29	\N	uplink	ISP12	19	ISP12	0	\N	\N	\N	3333:38070
8	1	209	6	6	198.51.0.31	\N	uplink	ISP13	18	ISP13	0	\N	\N	\N	3333:38080
2	0	215	1	1	198.51.0.1	\N	uplink	ISP0	20	ISP0	0	\N	\N	\N	3333:38020
9	2	226	10	6	198.51.0.33	\N	uplink	ISP4	20	ISP4	0	\N	\N	\N	3333:38090
10	2	227	11	8	198.51.0.35	\N	uplink	ISP5	20	ISP5	0	\N	\N	\N	3333:38100
0	1	0	0	0	\N	\N	\N	\N	21	\N	\N	\N	\N	\N	\N
\.


--
-- Name: pfr_peers_id_seq; Type: SEQUENCE SET; Schema: public; Owner: vc
--

SELECT pg_catalog.setval('pfr_peers_id_seq', 10, true);


--
-- Data for Name: pfr_prefix4; Type: TABLE DATA; Schema: public; Owner: vc
--

COPY pfr_prefix4 (id, customer_id, prefix) FROM stdin;
1	7	1.0.5.128/25
2	7	1.0.6.0/27
7	7	60.128.224.42/32
\.


--
-- Name: pfr_prefix4_id_seq; Type: SEQUENCE SET; Schema: public; Owner: vc
--

SELECT pg_catalog.setval('pfr_prefix4_id_seq', 7, true);


--
-- Data for Name: pfr_prefix6; Type: TABLE DATA; Schema: public; Owner: vc
--

COPY pfr_prefix6 (id, customer_id, prefix) FROM stdin;
\.


--
-- Name: pfr_prefix6_id_seq; Type: SEQUENCE SET; Schema: public; Owner: vc
--

SELECT pg_catalog.setval('pfr_prefix6_id_seq', 1, false);


--
-- Data for Name: pfr_results_mesurements; Type: TABLE DATA; Schema: public; Owner: vc
--

COPY pfr_results_mesurements (id, ipv4_dest_address, ipv6_dest_address, pkts, rtt_min, rtt_avg, rtt_max, rtt_mdev, pkt_loss, delay, jitter, latency, pfr_peers_id) FROM stdin;
\.


--
-- Name: pfr_results_mesurements_id_seq; Type: SEQUENCE SET; Schema: public; Owner: vc
--

SELECT pg_catalog.setval('pfr_results_mesurements_id_seq', 1, false);


--
-- Data for Name: sub_int_roles; Type: TABLE DATA; Schema: public; Owner: vc
--

COPY sub_int_roles (id, role) FROM stdin;
1	uplink
2	peer
3	customer
0	fake
\.


--
-- Name: sub_int_roles_id_seq; Type: SEQUENCE SET; Schema: public; Owner: vc
--

SELECT pg_catalog.setval('sub_int_roles_id_seq', 3, true);


--
-- Data for Name: sub_interfaces; Type: TABLE DATA; Schema: public; Owner: vc
--

COPY sub_interfaces (id, parent_id, type, intf, unit, role_id, description, snmp_idx, intf_idx) FROM stdin;
1	396	\N	\N	1	1	\N	2694	2518
3	396	\N	\N	2	1	\N	5560	2519
4	396	\N	\N	3	1	\N	5561	2520
2	397	\N	\N	10	1	\N	575	405
5	397	\N	\N	11	1	\N	1197	406
6	397	\N	\N	12	1	\N	1198	407
10	398	\N	\N	0	1	\N	2694	2518
11	399	\N	\N	0	1	\N	2694	2518
0	0	fake	\N	0	0	fake	\N	\N
\.


--
-- Name: sub_interfaces_id_seq; Type: SEQUENCE SET; Schema: public; Owner: vc
--

SELECT pg_catalog.setval('sub_interfaces_id_seq', 11, true);


--
-- Data for Name: type; Type: TABLE DATA; Schema: public; Owner: vc
--

COPY type (id, type, role, name) FROM stdin;
1	1	pe	pe
0	0	fake	fake
\.


--
-- Name: type_id_seq; Type: SEQUENCE SET; Schema: public; Owner: vc
--

SELECT pg_catalog.setval('type_id_seq', 1, true);


--
-- Data for Name: vc; Type: TABLE DATA; Schema: public; Owner: vc
--

COPY vc (id, circuit_id, vlan_id_outer, vlan_id_inner, description, vc_type_id) FROM stdin;
\.


--
-- Data for Name: vc_type; Type: TABLE DATA; Schema: public; Owner: vc
--

COPY vc_type (id, type) FROM stdin;
\.


--
-- Data for Name: vlans; Type: TABLE DATA; Schema: public; Owner: vc
--

COPY vlans (id, interface_id, node_id, unit, vid, vid_range0, vid_range1, name, sub_interface_id) FROM stdin;
\.


--
-- Data for Name: vrf; Type: TABLE DATA; Schema: public; Owner: vc
--

COPY vrf (id, description, name, vrf_type_id, rd, rt_import, rt_export, ltint, net, vlan_id) FROM stdin;
1	pfr01_icdn3_bgp	pfr01_icdn3_bgp	8	0	0	0	0	0	0
2	pfr00_icdn3_bgp	pfr00_icdn3_bgp	10	1156	1156	1156	0	0	0
3	icdn3	icdn3	11	2054	2054	2054	lt-0/0/10.1082	10.233.241.0	1082
4	pfr01_icdn2_bgp	pfr01_icdn2_bgp	8	0	0	0	0	0	0
5	pfr00_icdn2_bgp	pfr00_icdn2_bgp	10	1155	1155	1155	0	0	0
6	icdn2	icdn2	11	2053	2053	2053	lt-0/0/10.1078	10.233.240.0	1081
\.


--
-- Name: vrf_id_seq; Type: SEQUENCE SET; Schema: public; Owner: vc
--

SELECT pg_catalog.setval('vrf_id_seq', 6, true);


--
-- Data for Name: vrf_nodes; Type: TABLE DATA; Schema: public; Owner: vc
--

COPY vrf_nodes (id, vrf_id, node_id) FROM stdin;
1	6	215
2	6	209
3	6	213
4	6	210
5	3	215
6	3	209
7	3	213
8	3	210
\.


--
-- Name: vrf_nodes_id_seq; Type: SEQUENCE SET; Schema: public; Owner: vc
--

SELECT pg_catalog.setval('vrf_nodes_id_seq', 8, true);


--
-- Data for Name: vrf_type; Type: TABLE DATA; Schema: public; Owner: vc
--

COPY vrf_type (id, type, description) FROM stdin;
1	evpn	EVPN routing instance
2	forwarding	Forwarding instance
3	l2backhaul-vpn	L2Backhaul/L2Wholesale routing instance
4	l2vpn	Layer 2 VPN routing instance
5	layer2-control	Layer 2 control protocols
6	mpls-internet-multicast	mpls-internet-multicast
7	no-forwarding	Nonforwarding instance
8	virtual-router	Virtual routing instance
9	virtual-switch	Virtual switch routing instance
10	vpls	VPLS routing instance
11	vrf	Virtual routing forwarding instance
\.


--
-- Name: vrf_type_id_seq; Type: SEQUENCE SET; Schema: public; Owner: vc
--

SELECT pg_catalog.setval('vrf_type_id_seq', 11, true);


--
-- Data for Name: vrf_vrf; Type: TABLE DATA; Schema: public; Owner: vc
--

COPY vrf_vrf (id, vrf_id0, vrf_id1, vrf_id2) FROM stdin;
1	3	2	1
2	6	5	4
\.


--
-- Name: vrf_vrf_id_seq; Type: SEQUENCE SET; Schema: public; Owner: vc
--

SELECT pg_catalog.setval('vrf_vrf_id_seq', 2, true);


--
-- Name: adjs_pkey; Type: CONSTRAINT; Schema: public; Owner: vc; Tablespace: 
--

ALTER TABLE ONLY adjs
    ADD CONSTRAINT adjs_pkey PRIMARY KEY (id, node_id, adj_node_id, interface_id_oif, interface_id_iif);


--
-- Name: interface_pkey; Type: CONSTRAINT; Schema: public; Owner: vc; Tablespace: 
--

ALTER TABLE ONLY interface
    ADD CONSTRAINT interface_pkey PRIMARY KEY (id);


--
-- Name: ipam_addresses_pkey; Type: CONSTRAINT; Schema: public; Owner: vc; Tablespace: 
--

ALTER TABLE ONLY ipam_addresses
    ADD CONSTRAINT ipam_addresses_pkey PRIMARY KEY (id);


--
-- Name: ipam_vrf_pkey; Type: CONSTRAINT; Schema: public; Owner: vc; Tablespace: 
--

ALTER TABLE ONLY ipam_vrf
    ADD CONSTRAINT ipam_vrf_pkey PRIMARY KEY (id);


--
-- Name: node_pkey; Type: CONSTRAINT; Schema: public; Owner: vc; Tablespace: 
--

ALTER TABLE ONLY node
    ADD CONSTRAINT node_pkey PRIMARY KEY (id);


--
-- Name: pfr_case_pkey; Type: CONSTRAINT; Schema: public; Owner: vc; Tablespace: 
--

ALTER TABLE ONLY pfr_case
    ADD CONSTRAINT pfr_case_pkey PRIMARY KEY (id);


--
-- Name: pfr_customer_pkey; Type: CONSTRAINT; Schema: public; Owner: vc; Tablespace: 
--

ALTER TABLE ONLY pfr_customer
    ADD CONSTRAINT pfr_customer_pkey PRIMARY KEY (id);


--
-- Name: pfr_customer_type_pkey; Type: CONSTRAINT; Schema: public; Owner: vc; Tablespace: 
--

ALTER TABLE ONLY pfr_customer_type
    ADD CONSTRAINT pfr_customer_type_pkey PRIMARY KEY (id);


--
-- Name: pfr_dscp_pkey; Type: CONSTRAINT; Schema: public; Owner: vc; Tablespace: 
--

ALTER TABLE ONLY pfr_dscp
    ADD CONSTRAINT pfr_dscp_pkey PRIMARY KEY (id);


--
-- Name: pfr_dst_pkey; Type: CONSTRAINT; Schema: public; Owner: vc; Tablespace: 
--

ALTER TABLE ONLY pfr_dst
    ADD CONSTRAINT pfr_dst_pkey PRIMARY KEY (id);


--
-- Name: pfr_log_pkey; Type: CONSTRAINT; Schema: public; Owner: vc; Tablespace: 
--

ALTER TABLE ONLY pfr_log
    ADD CONSTRAINT pfr_log_pkey PRIMARY KEY (id);


--
-- Name: pfr_peer_group_pkey; Type: CONSTRAINT; Schema: public; Owner: vc; Tablespace: 
--

ALTER TABLE ONLY pfr_peer_group
    ADD CONSTRAINT pfr_peer_group_pkey PRIMARY KEY (id);


--
-- Name: pfr_peers_pkey; Type: CONSTRAINT; Schema: public; Owner: vc; Tablespace: 
--

ALTER TABLE ONLY pfr_peers
    ADD CONSTRAINT pfr_peers_pkey PRIMARY KEY (id);


--
-- Name: pfr_prefix4_pkey; Type: CONSTRAINT; Schema: public; Owner: vc; Tablespace: 
--

ALTER TABLE ONLY pfr_prefix4
    ADD CONSTRAINT pfr_prefix4_pkey PRIMARY KEY (id);


--
-- Name: pfr_prefix6_pkey; Type: CONSTRAINT; Schema: public; Owner: vc; Tablespace: 
--

ALTER TABLE ONLY pfr_prefix6
    ADD CONSTRAINT pfr_prefix6_pkey PRIMARY KEY (id);


--
-- Name: pfr_results_mesurements_pkey; Type: CONSTRAINT; Schema: public; Owner: vc; Tablespace: 
--

ALTER TABLE ONLY pfr_results_mesurements
    ADD CONSTRAINT pfr_results_mesurements_pkey PRIMARY KEY (id);


--
-- Name: sub_int_roles_pkey; Type: CONSTRAINT; Schema: public; Owner: vc; Tablespace: 
--

ALTER TABLE ONLY sub_int_roles
    ADD CONSTRAINT sub_int_roles_pkey PRIMARY KEY (id);


--
-- Name: sub_interfaces_pkey; Type: CONSTRAINT; Schema: public; Owner: vc; Tablespace: 
--

ALTER TABLE ONLY sub_interfaces
    ADD CONSTRAINT sub_interfaces_pkey PRIMARY KEY (id);


--
-- Name: type_pkey; Type: CONSTRAINT; Schema: public; Owner: vc; Tablespace: 
--

ALTER TABLE ONLY type
    ADD CONSTRAINT type_pkey PRIMARY KEY (id);


--
-- Name: vc_pkey; Type: CONSTRAINT; Schema: public; Owner: vc; Tablespace: 
--

ALTER TABLE ONLY vc
    ADD CONSTRAINT vc_pkey PRIMARY KEY (id);


--
-- Name: vc_type_pkey; Type: CONSTRAINT; Schema: public; Owner: vc; Tablespace: 
--

ALTER TABLE ONLY vc_type
    ADD CONSTRAINT vc_type_pkey PRIMARY KEY (id);


--
-- Name: vlans_pkey; Type: CONSTRAINT; Schema: public; Owner: vc; Tablespace: 
--

ALTER TABLE ONLY vlans
    ADD CONSTRAINT vlans_pkey PRIMARY KEY (id);


--
-- Name: vrf_nodes_pkey; Type: CONSTRAINT; Schema: public; Owner: vc; Tablespace: 
--

ALTER TABLE ONLY vrf_nodes
    ADD CONSTRAINT vrf_nodes_pkey PRIMARY KEY (id);


--
-- Name: vrf_pkey; Type: CONSTRAINT; Schema: public; Owner: vc; Tablespace: 
--

ALTER TABLE ONLY vrf
    ADD CONSTRAINT vrf_pkey PRIMARY KEY (id);


--
-- Name: vrf_type_pkey; Type: CONSTRAINT; Schema: public; Owner: vc; Tablespace: 
--

ALTER TABLE ONLY vrf_type
    ADD CONSTRAINT vrf_type_pkey PRIMARY KEY (id);


--
-- Name: vrf_vrf_pkey; Type: CONSTRAINT; Schema: public; Owner: vc; Tablespace: 
--

ALTER TABLE ONLY vrf_vrf
    ADD CONSTRAINT vrf_vrf_pkey PRIMARY KEY (id);


--
-- Name: adjs_adj_node_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: vc
--

ALTER TABLE ONLY adjs
    ADD CONSTRAINT adjs_adj_node_id_fkey FOREIGN KEY (adj_node_id) REFERENCES node(id);


--
-- Name: adjs_interface_id_iif_fkey; Type: FK CONSTRAINT; Schema: public; Owner: vc
--

ALTER TABLE ONLY adjs
    ADD CONSTRAINT adjs_interface_id_iif_fkey FOREIGN KEY (interface_id_iif) REFERENCES interface(id);


--
-- Name: adjs_interface_id_oif_fkey; Type: FK CONSTRAINT; Schema: public; Owner: vc
--

ALTER TABLE ONLY adjs
    ADD CONSTRAINT adjs_interface_id_oif_fkey FOREIGN KEY (interface_id_oif) REFERENCES interface(id);


--
-- Name: adjs_node_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: vc
--

ALTER TABLE ONLY adjs
    ADD CONSTRAINT adjs_node_id_fkey FOREIGN KEY (node_id) REFERENCES node(id);


--
-- Name: interface_node_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: vc
--

ALTER TABLE ONLY interface
    ADD CONSTRAINT interface_node_id_fkey FOREIGN KEY (node_id) REFERENCES node(id);


--
-- Name: ipam_addresses_interface_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: vc
--

ALTER TABLE ONLY ipam_addresses
    ADD CONSTRAINT ipam_addresses_interface_id_fkey FOREIGN KEY (interface_id) REFERENCES sub_interfaces(id);


--
-- Name: ipam_addresses_vrf_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: vc
--

ALTER TABLE ONLY ipam_addresses
    ADD CONSTRAINT ipam_addresses_vrf_id_fkey FOREIGN KEY (vrf_id) REFERENCES ipam_vrf(id);


--
-- Name: node_type_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: vc
--

ALTER TABLE ONLY node
    ADD CONSTRAINT node_type_id_fkey FOREIGN KEY (type_id) REFERENCES type(id);


--
-- Name: pfr_customer_type_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: vc
--

ALTER TABLE ONLY pfr_customer
    ADD CONSTRAINT pfr_customer_type_id_fkey FOREIGN KEY (type_id) REFERENCES pfr_customer_type(id);


--
-- Name: pfr_log_ccase_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: vc
--

ALTER TABLE ONLY pfr_log
    ADD CONSTRAINT pfr_log_ccase_id_fkey FOREIGN KEY (ccase_id) REFERENCES pfr_case(id);


--
-- Name: pfr_log_peer_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: vc
--

ALTER TABLE ONLY pfr_log
    ADD CONSTRAINT pfr_log_peer_id_fkey FOREIGN KEY (peer_id) REFERENCES pfr_peers(id);


--
-- Name: pfr_peers_address_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: vc
--

ALTER TABLE ONLY pfr_peers
    ADD CONSTRAINT pfr_peers_address_id_fkey FOREIGN KEY (address_id) REFERENCES ipam_addresses(id);


--
-- Name: pfr_peers_dscp_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: vc
--

ALTER TABLE ONLY pfr_peers
    ADD CONSTRAINT pfr_peers_dscp_id_fkey FOREIGN KEY (dscp_id) REFERENCES pfr_dscp(id);


--
-- Name: pfr_peers_interface_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: vc
--

ALTER TABLE ONLY pfr_peers
    ADD CONSTRAINT pfr_peers_interface_id_fkey FOREIGN KEY (interface_id) REFERENCES sub_interfaces(id);


--
-- Name: pfr_peers_node_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: vc
--

ALTER TABLE ONLY pfr_peers
    ADD CONSTRAINT pfr_peers_node_id_fkey FOREIGN KEY (node_id) REFERENCES node(id);


--
-- Name: pfr_prefix4_customer_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: vc
--

ALTER TABLE ONLY pfr_prefix4
    ADD CONSTRAINT pfr_prefix4_customer_id_fkey FOREIGN KEY (customer_id) REFERENCES pfr_customer(id);


--
-- Name: pfr_prefix6_customer_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: vc
--

ALTER TABLE ONLY pfr_prefix6
    ADD CONSTRAINT pfr_prefix6_customer_id_fkey FOREIGN KEY (customer_id) REFERENCES pfr_customer(id);


--
-- Name: pfr_results_mesurements_pfr_peers_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: vc
--

ALTER TABLE ONLY pfr_results_mesurements
    ADD CONSTRAINT pfr_results_mesurements_pfr_peers_id_fkey FOREIGN KEY (pfr_peers_id) REFERENCES pfr_peers(id);


--
-- Name: sub_interfaces_parent_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: vc
--

ALTER TABLE ONLY sub_interfaces
    ADD CONSTRAINT sub_interfaces_parent_id_fkey FOREIGN KEY (parent_id) REFERENCES interface(id);


--
-- Name: sub_interfaces_role_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: vc
--

ALTER TABLE ONLY sub_interfaces
    ADD CONSTRAINT sub_interfaces_role_id_fkey FOREIGN KEY (role_id) REFERENCES sub_int_roles(id);


--
-- Name: vc_vc_type_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: vc
--

ALTER TABLE ONLY vc
    ADD CONSTRAINT vc_vc_type_id_fkey FOREIGN KEY (vc_type_id) REFERENCES vc_type(id);


--
-- Name: vc_vlan_id_inner_fkey; Type: FK CONSTRAINT; Schema: public; Owner: vc
--

ALTER TABLE ONLY vc
    ADD CONSTRAINT vc_vlan_id_inner_fkey FOREIGN KEY (vlan_id_inner) REFERENCES vlans(id);


--
-- Name: vc_vlan_id_outer_fkey; Type: FK CONSTRAINT; Schema: public; Owner: vc
--

ALTER TABLE ONLY vc
    ADD CONSTRAINT vc_vlan_id_outer_fkey FOREIGN KEY (vlan_id_outer) REFERENCES vlans(id);


--
-- Name: vlans_interface_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: vc
--

ALTER TABLE ONLY vlans
    ADD CONSTRAINT vlans_interface_id_fkey FOREIGN KEY (interface_id) REFERENCES interface(id);


--
-- Name: vlans_node_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: vc
--

ALTER TABLE ONLY vlans
    ADD CONSTRAINT vlans_node_id_fkey FOREIGN KEY (node_id) REFERENCES node(id);


--
-- Name: vlans_sub_interface_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: vc
--

ALTER TABLE ONLY vlans
    ADD CONSTRAINT vlans_sub_interface_id_fkey FOREIGN KEY (sub_interface_id) REFERENCES interface(id);


--
-- Name: vrf_nodes_node_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: vc
--

ALTER TABLE ONLY vrf_nodes
    ADD CONSTRAINT vrf_nodes_node_id_fkey FOREIGN KEY (node_id) REFERENCES node(id);


--
-- Name: vrf_vrf_type_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: vc
--

ALTER TABLE ONLY vrf
    ADD CONSTRAINT vrf_vrf_type_id_fkey FOREIGN KEY (vrf_type_id) REFERENCES vrf_type(id);


--
-- Name: vrf_vrf_vrf_id1_fkey; Type: FK CONSTRAINT; Schema: public; Owner: vc
--

ALTER TABLE ONLY vrf_vrf
    ADD CONSTRAINT vrf_vrf_vrf_id1_fkey FOREIGN KEY (vrf_id1) REFERENCES vrf(id);


--
-- Name: vrf_vrf_vrf_id2_fkey; Type: FK CONSTRAINT; Schema: public; Owner: vc
--

ALTER TABLE ONLY vrf_vrf
    ADD CONSTRAINT vrf_vrf_vrf_id2_fkey FOREIGN KEY (vrf_id2) REFERENCES vrf(id);


--
-- Name: public; Type: ACL; Schema: -; Owner: postgres
--

REVOKE ALL ON SCHEMA public FROM PUBLIC;
REVOKE ALL ON SCHEMA public FROM postgres;
GRANT ALL ON SCHEMA public TO postgres;
GRANT ALL ON SCHEMA public TO PUBLIC;


--
-- PostgreSQL database dump complete
--

