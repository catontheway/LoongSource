--ʹ�����꼤��ͼ��3200567��ѧ�Ἴ�����꼤��9001401
--ʹ��Ч��
function i3200567_Usable(MapID, InstanceID, TypeID, TargetID)
	role.AddSkill(TargetID, 9001401)
end

--ע��
aux.RegisterItemEvent(3200567, 1, "i3200567_Usable")

function I3200567_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�ж�����Ƿ���ѧ����̼���

	local LearnedSkill = role.IsLearnedSkill(TargetID, 90014)

	if LearnedSkill~=nil then
		--��ʾ�����Ʒ����ʹ��
		bRet = 32
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(3200567, 0, "I3200567_CanUse")
