function I3200463_SkillBook(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��

	role.AddSkill(TargetID, 1010701)

end

function I3200463_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�ж�����Ƿ���ѧ�����񹦼���

	local LearnedSkill = role.IsLearnedSkill(TargetID, 10107)

	if LearnedSkill~=nil then
		--��ʾ�����Ʒ����ʹ��
		bRet = 32
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 10001)
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(3200463, 1, "I3200463_SkillBook")
aux.RegisterItemEvent(3200463, 0, "I3200463_CanUse")
