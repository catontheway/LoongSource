--��ħʯ��12216
--����1���ڵ����ħʯ��������ʹ�õ��߷�ħ��[2612138]

--���߷�ħ��ʹ���жϽű�����
function i2612138_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false

	--������¹����ʹ��
	if MapID ~= 3017299663 then
	    bRet = 43
	else
	    local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
	    if x<=1936 or x>=1968 or z<=1991 or z >=2023 then --�����ڸķ�Χ��ʹ��
	        bRet = 43
	    end
	end

    --����
	return bRet, bIgnore
end

--ע��
aux.RegisterItemEvent(2612138, 0, "i2612138_CanUse")


function x12216_OnNPCTalk(mapID, instanceID, questID, ownerID, npcid, npctypeid)
	if npctypeid == 3030122 then
	unit.AddBuff(mapID, instanceID, ownerID, 2012101, ownerID)
	end
end

--ע��
aux.RegisterQuestEvent(12216, 6, "x12216_OnNPCTalk")
