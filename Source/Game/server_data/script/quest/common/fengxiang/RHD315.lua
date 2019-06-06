function x20315_OnCheckAccept(MapID, InstanceID, QuestID, RoleID, NPCID)
	if role.GetRoleScriptData(RoleID, 1, RoleDataType["Q20315_Times"]) >= 3 then
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26,	3448)		--ÿ�����ֻ�����3�θ��������Ѿ��ﵽ�����ޣ��޷�������ȡ
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
        return 0
	end
	return 1
end

aux.RegisterQuestEvent(20315, 4, "x20315_OnCheckAccept")

--�������
function x20315_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)

	role.SetRoleScriptData(RoleID, 1, RoleDataType["Q20315_Times"], role.GetRoleScriptData(RoleID, 1, RoleDataType["Q20315_Times"])+1)

end

aux.RegisterQuestEvent(20315, 1, "x20315_OnComplete")
