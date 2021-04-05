export interface Decoder {
    id: string;
    name: string;
    longname: string;
    desc: string;
}

export interface DecoderOption {
    id: string;
    desc: string;
    default: any;
    values: any[];
}

export interface DecoderChannel {
    id: string;
    name: string;
    desc: string;
    isOptional: boolean;
}

export interface AcquirerOptions {
    samplerates: string[];
    gains: string[];
    maxSampleCount: string;
    availableChannels: string[];
}