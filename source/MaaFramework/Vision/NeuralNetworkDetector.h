#pragma once

#include <ostream>
#include <vector>

#include <onnxruntime/onnxruntime_cxx_api.h>

#include "VisionBase.h"
#include "VisionTypes.h"

MAA_VISION_NS_BEGIN

class NeuralNetworkDetector : public VisionBase
{
public:
    struct Result
    {
        size_t cls_index = SIZE_MAX;
        std::string label;
        cv::Rect box {};
        double score = 0.0;

        json::value to_json() const
        {
            json::value root;
            root["cls_index"] = cls_index;
            root["label"] = label;
            root["box"] = json::array({ box.x, box.y, box.width, box.height });
            root["score"] = score;
            return root;
        }
    };
    using ResultsVec = std::vector<Result>;

public:
    void set_session(std::shared_ptr<Ort::Session> session) { session_ = std::move(session); }
    void set_param(NeuralNetworkDetectorParam param) { param_ = std::move(param); }
    ResultsVec analyze() const;

private:
    ResultsVec foreach_rois() const;
    ResultsVec detect(const cv::Rect& roi) const;
    void draw_result(const cv::Rect& roi, const ResultsVec& results) const;

    void filter(ResultsVec& results, const std::vector<size_t>& expected) const;

    NeuralNetworkDetectorParam param_;
    std::shared_ptr<Ort::Session> session_ = nullptr;
};

MAA_VISION_NS_END

MAA_NS_BEGIN

inline std::ostream& operator<<(std::ostream& os, const MAA_VISION_NS::NeuralNetworkDetector::Result& res)
{
    os << res.to_json().to_string();
    return os;
}

MAA_NS_END