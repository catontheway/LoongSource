function I4200000_SkillBook(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��

	role.AddSkill(TargetID, 9001201)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 26, 2220)
	msg.DispatchRoleMsgEvent(ownerID, MsgID)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 0)
	msg.AddMsgEvent(MsgID, 1, 2220)
	msg.DispatchRoleMsgEvent(ownerID, MsgID)

end

function I4200000_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�ж�����Ƿ���ѧ����̼���

	local LearnedSkill = role.IsLearnedSkill(TargetID, 90012)

	if LearnedSkill~=nil then
		--��ʾ�����Ʒ����ʹ��
		bRet = 32
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 10001)
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4200000, 1, "I4200000_SkillBook")
aux.RegisterItemEvent(4200000, 0, "I4200000_CanUse")
