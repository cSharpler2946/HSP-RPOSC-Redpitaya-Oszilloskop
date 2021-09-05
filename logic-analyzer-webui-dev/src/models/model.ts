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
    samplerates_Hz: number[];
    gains: string[];
    maxSampleCount: number;
    availableChannels: string[];
}

export interface AcquirerChosenOptions {
    samplerate_Hz: number,
    samplecount: number,
    sampletime_us: number,
    gainPerChannel: Record<string, string>,
    probeAttenuationPerChannel: Record<string, string>,
}

export interface DecoderChannelTuple {
    acqChannel: string,
    srdChannel: string
}

export interface LogicSession {
    measurementState: MeasurementState
}

export enum MeasurementState {
    Stopped = "stopped",
    Starting = "starting",
    Running = "running"
}

export interface MeasuredData {
    channelData: ChannelData[]
}

export interface ChannelData {
    acqChannel: string,
    data: number[]
}