#include"MrpcController.h"
void MrpcController::SetFailed(const std::string& reason)
{
    isfail=true;
    failtext=reason;
}
bool MrpcController::Failed() const
{
    return isfail;
}
void MrpcController::Reset()
{
    isfail=false;
    failtext="";
}
std::string MrpcController::ErrorText() const
{
    return failtext;
}
void MrpcController::StartCancel()
{
;
}
bool MrpcController::IsCanceled() const
{
    return true;
}
void MrpcController::NotifyOnCancel(google::protobuf::Closure* callback)
{
    return;
}