--ʹ�����걬��ͼ��3200566��ѧ�Ἴ�����걬��9001301
--ʹ��Ч��
function i3200566_Usable(MapID, InstanceID, TypeID, TargetID)
	role.AddSkill(TargetID, 9001301)
end

--ע��
aux.RegisterItemEvent(3200566, 1, "i3200566_Usable")


function I3200566_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�ж�����Ƿ���ѧ����̼���

	local LearnedSkill = role.IsLearnedSkill(TargetID, 90013)

	if LearnedSkill~=nil then
		--��ʾ�����Ʒ����ʹ��
		bRet = 32
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(3200566, 0, "I3200566_CanUse")
