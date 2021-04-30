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

export interface AcquirerRequestedOptions {
    samplerates: string[];
    gains: string[];
    maxSampleCount: string;
    availableChannels: string[];
}

export interface AcquirerChosenOptions {
    samplerate: string,
    samplecount: string,
    sampletime: string,
    gainPerChannel: Record<string, string>,
    probeAttenuationPerChannel: Record<string, string>,
}
