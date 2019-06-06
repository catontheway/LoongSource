function ShengChanXinDe_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�ж�����Ƿ���ѧ����̼���

	local Produce_Skill_Num=0

	for i=0, 9 do
		local LearnedSkill = role.IsLearnedSkill(TargetID, 90000+i)
		if LearnedSkill~=nil then
			Produce_Skill_Num = Produce_Skill_Num +1
		end
	end

	if Produce_Skill_Num==0 then
		bRet = 32
		--��ʾ���û��ѧ����������
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 10001)
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
	end

	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < Produce_Skill_Num) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end

	return bRet, bIgnore

end

function ShengChanXinDe(MapID, InstanceID, TypeID, TargetID)

	for i=0, 9 do
		local LearnedSkill = role.IsLearnedSkill(TargetID, 90000+i)
		if LearnedSkill~=nil then
			role.AddRoleItem(MapID, InstanceID, TargetID, 3200226+i, 1, -1, 8, 420)
		end
	end

end

aux.RegisterItemEvent(3200215, 0, "ShengChanXinDe_CanUse")
aux.RegisterItemEvent(3200215, 1, "ShengChanXinDe")
