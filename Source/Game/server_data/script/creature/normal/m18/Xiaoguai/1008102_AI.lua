-- �������ս������ʾ��
function c1008102_OnEnterCombat(MapID, InstanceID, CreatureID)

    local Temp = map.GetMapScriptData(MapID, InstanceID, 1, 16)

	if Temp~=1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 1799)	--��Ԫ����30���Ӻ���ʧ���뾡���ȡ���е���ħ���ԶԿ�����߳����
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

		map.SetMapScriptData(MapID, InstanceID, 1, 16, 1)
	end
end

aux.RegisterCreatureEvent(1008102, 2, "c1008102_OnEnterCombat")





