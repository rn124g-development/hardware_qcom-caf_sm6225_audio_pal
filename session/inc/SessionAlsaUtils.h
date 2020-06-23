/*
 * Copyright (c) 2019, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of The Linux Foundation nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SESSION_ALSAUTILS_H
#define SESSION_ALSAUTILS_H

#include "Session.h"
#include "ResourceManager.h"
#include "PayloadBuilder.h"

#include <tinyalsa/asoundlib.h>
#include <sound/asound.h>


class Stream;
class Session;

enum class MixerCtlType: uint32_t {
    MIXER_SET_ID_STRING,
    MIXER_SET_ID_VALUE,
    MIXER_SET_ID_ARRAY,
};

enum FeCtrlsIndex {
    FE_CONTROL,
    FE_METADATA,
    FE_CONNECT,
    FE_DISCONNECT,
    FE_SETPARAM,
    FE_GETTAGGEDINFO,
    FE_SETPARAMTAG,
    FE_GETPARAM,
    FE_ECHOREFERENCE,
    FE_SIDETONE,
    FE_LOOPBACK,
    FE_EVENT,
    FE_SETCAL,
    FE_MAX_NUM_MIXER_CONTROLS,
};

enum BeCtrlsIndex {
    BE_METADATA,
    BE_MEDIAFMT,
    BE_SETPARAM,
    BE_MAX_NUM_MIXER_CONTROLS,
};


class SessionAlsaUtils
{
private:
    SessionAlsaUtils() {};
    static bool isRxDevice(uint32_t devId);
    static struct mixer_ctl *getFeMixerControl(struct mixer *am, std::string feName,
        uint32_t idx);
    static struct mixer_ctl *getBeMixerControl(struct mixer *am, std::string beName,
        uint32_t idx);
public:
    ~SessionAlsaUtils();
    static int setMixerCtlData(struct mixer_ctl *ctl, MixerCtlType id, void *data, int size);
    static int getTagMetadata(int32_t tagsent, std::vector <std::pair<int, int>> &tkv, struct agm_tag_config *tagConfig);
    static int getCalMetadata(std::vector <std::pair<int, int>> &ckv, struct agm_cal_config* calConfig);
    static unsigned int bitsToAlsaFormat(unsigned int bits);
    static int open(Stream * s, std::shared_ptr<ResourceManager> rm, const std::vector<int> &DevIds,
            const std::vector<std::pair<int32_t, std::string>> &BackEnds);
    static int open(Stream * s, std::shared_ptr<ResourceManager> rm,
                    const std::vector<int> &RxDevIds, const std::vector<int> &TxDevIds,
                    const std::vector<std::pair<int32_t, std::string>> &rxBackEnds,
                    const std::vector<std::pair<int32_t, std::string>> &txBackEnds);
    static int close(Stream * s, std::shared_ptr<ResourceManager> rm, const std::vector<int> &DevIds,
            const std::vector<std::pair<int32_t, std::string>> &BackEnds, std::vector<std::pair<std::string, int>> &freedevicemetadata);
    static int close(Stream * s, std::shared_ptr<ResourceManager> rm,
                    const std::vector<int> &RxDevIds, const std::vector<int> &TxDevIds,
                    const std::vector<std::pair<int32_t, std::string>> &rxBackEnds,
                    const std::vector<std::pair<int32_t, std::string>> &txBackEnds);
    static int getModuleInstanceId(struct mixer *mixer, int device, const char *intf_name,
                       int tag_id, uint32_t *miid);
    static int setMixerParameter(struct mixer *mixer, int device,
                                 void *payload, int size);
    static int setStreamMetadataType(struct mixer *mixer, int device, const char *val);
    static int registerMixerEvent(struct mixer *mixer, int device, const char *intf_name, int tag_id, void *payload, int payload_size);
    static int setECRefPath(struct mixer *mixer, int device, const char *intf_name);

    static int getTimestamp(struct mixer *mixer, const std::vector<int> &DevIds, uint32_t spr_miid, struct qal_session_time *stime);
    static int disconnectSessionDevice(Stream* streamHandle, qal_stream_type_t streamType,
        std::shared_ptr<ResourceManager> rm, struct qal_device &dAttr,
        const std::vector<int> &pcmDevIds,
        const std::vector<std::pair<int32_t, std::string>> &aifBackEndsToDisconnect);
    static int connectSessionDevice(Session* sess, Stream* streamHandle, qal_stream_type_t streamType,
        std::shared_ptr<ResourceManager> rm, struct qal_device &dAttr,
        const std::vector<int> &pcmDevIds,
        const std::vector<std::pair<int32_t, std::string>> &aifBackEndsToConnect);
    static int setupSessionDevice(Stream* streamHandle, qal_stream_type_t streamType,
        std::shared_ptr<ResourceManager> rm, struct qal_device &dAttr,
        const std::vector<int> &pcmDevIds,
        const std::vector<std::pair<int32_t, std::string>> &aifBackEndsToConnect);
    static std::shared_ptr<Device> getDeviceObj(int32_t beDevId,
        std::vector<std::shared_ptr<Device>> &associatedDevices);
    static int setDeviceMetadata(std::shared_ptr<ResourceManager> rmHandle,
                                std::string backEndName,
                                std::vector <std::pair<int, int>> &deviceKV);
    static int setDeviceMediaConfig(std::shared_ptr<ResourceManager> rmHandle,
                            std::string backEndName, struct qal_device *dAttr);
    static int setDeviceCustomPayload(std::shared_ptr<ResourceManager> rmHandle,
                           std::string backEndName, void *payload, size_t size);
    static unsigned int bytesToFrames(size_t bufSizeInBytes, unsigned int channels,
                           enum pcm_format format);
    static bool isMmapUsecase(struct qal_stream_attributes attr);
    static void getAgmMetaData(const std::vector <std::pair<int, int>> &kv,
                        const std::vector <std::pair<int, int>> &ckv,
                        struct prop_data *propData,
                        struct agmMetaData &md);
};

#endif //SESSION_ALSA_UTILS
